#include "Plugin/Internal/IO/JsonFile.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/Script/ScriptVariantMap.hpp"

namespace Plugin::Internal::IO::Impl
{
	[[nodiscard]] static auto GetKeyString(const RE::BSScript::Variable& a_var) -> std::optional<std::string_view>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_var.GetRawType()) {
			case RawType::kString: {
				return a_var.GetString();
			}
			case RawType::kVar: {
				const auto* var = a_var.GetVar();
				if (!var) {
					return std::nullopt;
				}

				return GetKeyString(*var);
			}
			default: {
				return std::nullopt;
			}
		}
	}
}

namespace Plugin::Internal::IO
{
	JsonFile::JsonFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex)
		: _ioMutex(a_ioMutex)
	{
	}

	JsonFile::~JsonFile() noexcept = default;

	bool JsonFile::ReadFile(const std::filesystem::path& a_path, json_t& a_root) const
	{
		const auto ioLock = _ioMutex->LockShared(a_path);

		try {
			auto fileStream = std::fstream(a_path, std::ios::in);
			a_root = json_t::parse(fileStream,
				Core::JsonUtility::GetParserCallback(),
				Core::JsonUtility::ALLOW_EXCEPTIONS,
				Core::JsonUtility::IGNORE_COMMENTS);
			return true;
		}
		catch ([[maybe_unused]] const json_t::exception& error) {
			a_root = json_t();
			return false;
		}
	}

	bool JsonFile::WriteFile(const std::filesystem::path& a_path, const json_t& a_root) const
	{
		const auto ioLock = _ioMutex->LockUnique(a_path);

		try {
			auto fileStream = std::fstream(a_path, std::ios::out | std::ios::trunc);
			REX::Print(fileStream, "{}"sv,
				a_root.dump(
					Core::JsonUtility::INDENT_COUNT,
					Core::JsonUtility::INDENT_CHAR,
					Core::JsonUtility::ENSURE_ASCII,
					Core::JsonUtility::ERROR_HANDLER));
			return true;
		}
		catch ([[maybe_unused]] const json_t::exception& error) {
			return false;
		}
	}

	auto JsonFile::Deserialize(const std::filesystem::path& a_path, const RE::BSFixedString& a_structName) const -> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>
	{
		auto jsonRoot = json_t();
		if (!ReadFile(a_path, jsonRoot)) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!scriptVM->GetScriptStructType(a_structName, structTypeInfo) || !structTypeInfo) {
			return std::nullopt;
		}

		const auto typeInfo = RE::BSScript::TypeInfo(structTypeInfo.get());
		return Core::JsonUtility::ToStruct(jsonRoot, typeInfo);
	}

	bool JsonFile::Serialize(const std::filesystem::path& a_path, const RE::BSTSmartPointer<RE::BSScript::Struct>& a_struct) const
	{
		auto jsonStruct = Core::JsonUtility::FromStruct(a_struct);
		if (!jsonStruct.has_value()) {
			return false;
		}

		return WriteFile(a_path, jsonStruct.value());
	}

	bool JsonFile::IsCachedAsVariantMap(const Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path)
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.ContainsNamespace(name);
	}

	bool JsonFile::CacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return CacheAsVariantMapTo(a_map, a_path, RE::BSFixedString(a_path.lexically_normal().generic_string()));
	}

	bool JsonFile::CacheAsVariantMapTo(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_sourcePath, const RE::BSFixedString& a_targetNamespace) const
	{
		auto jsonRoot = json_t();
		if (!ReadFile(a_sourcePath, jsonRoot)) {
			return false;
		}

		auto scriptPairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();
		scriptPairs.reserve(jsonRoot.size());

		for (const auto& [jsonKey, jsonValue] : jsonRoot.flatten().items()) {
			auto scriptKey = Core::JsonUtility::ToValue(jsonKey, RE::BSScript::TypeInfo::RawType::kString);
			if (!scriptKey.has_value()) {
				continue;
			}

			auto scriptVar = Core::JsonUtility::ToValue(jsonValue, RE::BSScript::TypeInfo::RawType::kVar);
			if (!scriptVar.has_value()) {
				continue;
			}

			scriptPairs.emplace_back(std::move(scriptKey).value(), std::move(scriptVar).value());
		}

		a_map.SetRange(a_targetNamespace, scriptPairs);
		return true;
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	bool JsonFile::UncacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.RemoveNamespace(name);
	}

	bool JsonFile::SaveCachedAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return SaveCachedAsVariantMapTo(a_map, RE::BSFixedString(a_path.lexically_normal().generic_string()), a_path);
	}

	bool JsonFile::SaveCachedAsVariantMapTo(Script::ScriptVariantMap& a_map, const RE::BSFixedString& a_sourceNamespace, const std::filesystem::path& a_targetPath) const
	{
		if (!a_map.ContainsNamespace(a_sourceNamespace)) {
			return false;
		}

		auto jsonRoot = json_t();
		a_map.ForEach(a_sourceNamespace, [&jsonRoot](const RE::BSScript::Variable& a_key, const RE::BSScript::Variable& a_value) {
			const auto keyString = Impl::GetKeyString(a_key);
			if (!keyString.has_value()) {
				return true;
			}

			const auto jsonPtr = Core::JsonUtility::GetPointer(keyString.value());
			if (!jsonPtr.has_value()) {
				return true;
			}

			auto jsonValue = Core::JsonUtility::FromValue(a_value);
			if (!jsonValue.has_value()) {
				return true;
			}

			jsonRoot[jsonPtr.value()] = std::move(jsonValue).value();
			return true;
		});

		return WriteFile(a_targetPath, jsonRoot);
	}
}
