#include "Plugin/Internal/IO/TomlFile.hpp"

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
	TomlFile::TomlFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex)
		: _ioMutex(a_ioMutex)
	{
	}

	TomlFile::~TomlFile() noexcept = default;

	bool TomlFile::ReadFile(const std::filesystem::path& a_path, toml_t& a_root) const
	{
		const auto ioLock = _ioMutex->LockShared(a_path);

		try {
			a_root = toml::parse(a_path, Core::TomlUtility::SPECIFICATION_VERSION);
			return true;
		}
		catch ([[maybe_unused]] const toml::exception& error) {
			a_root = toml_t();
			return false;
		}
	}

	bool TomlFile::WriteFile(const std::filesystem::path& a_path, const toml_t& a_root) const
	{
		const auto ioLock = _ioMutex->LockUnique(a_path);

		try {
			auto fileStream = std::fstream(a_path, std::ios::out | std::ios::trunc);
			REX::Print(fileStream, "{}"sv, toml::format(a_root, Core::TomlUtility::SPECIFICATION_VERSION));
			return fileStream.good();
		}
		catch ([[maybe_unused]] const toml::exception& error) {
			return false;
		}
	}

	bool TomlFile::IsCachedAsVariantMap(const Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path)
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.ContainsNamespace(name);
	}

	bool TomlFile::CacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return CacheAsVariantMapTo(a_map, a_path, RE::BSFixedString(a_path.lexically_normal().generic_string()));
	}

	bool TomlFile::CacheAsVariantMapTo(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_sourcePath, const RE::BSFixedString& a_targetNamespace) const
	{
		auto tomlRoot = toml_t();
		if (!ReadFile(a_sourcePath, tomlRoot) || !tomlRoot.is_table()) {
			return false;
		}

		auto flattenedRoot = REX::Toml::Flatten(tomlRoot);
		if (!flattenedRoot.has_value()) {
			return false;
		}

		auto scriptPairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();
		scriptPairs.reserve(tomlRoot.as_table().size());

		for (const auto& [tomlKey, tomlValue] : flattenedRoot->as_table()) {
			auto scriptKey = Core::TomlUtility::ToValue(tomlKey, RE::BSScript::TypeInfo::RawType::kString);
			if (!scriptKey.has_value()) {
				return false;
			}

			auto scriptValue = Core::TomlUtility::ToValue(tomlValue, RE::BSScript::TypeInfo::RawType::kVar);
			if (!scriptValue.has_value()) {
				return false;
			}

			scriptPairs.emplace_back(std::move(scriptKey).value(), std::move(scriptValue).value());
		}

		a_map.SetRange(a_targetNamespace, scriptPairs);
		return true;
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	bool TomlFile::UncacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.RemoveNamespace(name);
	}

	bool TomlFile::SaveCachedAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return SaveCachedAsVariantMapTo(a_map, RE::BSFixedString(a_path.lexically_normal().generic_string()), a_path);
	}

	bool TomlFile::SaveCachedAsVariantMapTo(Script::ScriptVariantMap& a_map, const RE::BSFixedString& a_sourceNamespace, const std::filesystem::path& a_targetPath) const
	{
		if (!a_map.ContainsNamespace(a_sourceNamespace)) {
			return false;
		}

		auto tomlRoot = toml_t{};
		a_map.ForEach(a_sourceNamespace, [&tomlRoot](const RE::BSScript::Variable& a_key, const RE::BSScript::Variable& a_value) {
			const auto keyString = Impl::GetKeyString(a_key);
			if (!keyString.has_value()) {
				return true;
			}

			const auto&& [tableName, keyName] = REX::Toml::SplitSetting(keyString.value());

			auto* tomlPtr = REX::Toml::GetOrCreatePointer(tomlRoot, tableName);
			if (!tomlPtr || !tomlPtr->is_table()) {
				return true;
			}

			auto tomlValue = Core::TomlUtility::FromValue(a_value);
			if (!tomlValue.has_value()) {
				return true;
			}

			tomlPtr->as_table().insert_or_assign(std::string(keyName), std::move(tomlValue).value());
			return true;
		});

		return WriteFile(a_targetPath, tomlRoot);
	}
}
