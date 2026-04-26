#include "Plugin/Internal/IO/IniFile.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/Script/ScriptVariantMap.hpp"

namespace Plugin::Internal::IO::Impl
{
	[[nodiscard]] static auto GetKeyString(const RE::BSScript::Variable& a_var) -> std::optional<REX::zstring_view>
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
	IniFile::IniFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex)
		: _ioMutex(a_ioMutex)
	{
	}

	IniFile::~IniFile() noexcept = default;

	void IniFile::SetupFile(ini_t& a_file)
	{
		Core::IniUtility::SetupFile(a_file);
	}

	bool IniFile::ReadFile(const std::filesystem::path& a_path, ini_t& a_ini) const
	{
		const auto ioLock = _ioMutex->LockShared(a_path);
		return a_ini.LoadFile(a_path.native().data()) >= SI_OK;
	}

	bool IniFile::WriteFile(const std::filesystem::path& a_path, const ini_t& a_ini) const
	{
		const auto ioLock = _ioMutex->LockUnique(a_path);
		return a_ini.SaveFile(a_path.native().data(), Core::IniUtility::PREPEND_BOM) >= SI_OK;
	}

	bool IniFile::IsCachedAsVariantMap(const Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path)
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.ContainsNamespace(name);
	}

	bool IniFile::CacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return CacheAsVariantMapTo(a_map, a_path, RE::BSFixedString(a_path.lexically_normal().generic_string()));
	}

	bool IniFile::CacheAsVariantMapTo(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_sourcePath, const RE::BSFixedString& a_targetNamespace) const
	{
		auto iniFile = ini_t();
		SetupFile(iniFile);

		if (!ReadFile(a_sourcePath, iniFile)) {
			return false;
		}

		auto iniSections = ini_t::TNamesDepend();
		iniFile.GetAllSections(iniSections);

		auto scriptPairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();

		for (const auto& section : iniSections) {
			auto iniKeys = ini_t::TNamesDepend();
			iniFile.GetAllKeys(section.pItem, iniKeys);

			scriptPairs.reserve(scriptPairs.size() + iniKeys.size());

			for (const auto& key : iniKeys) {
				auto iniValue = REX::Ini::GetValue<REX::zstring_view>(iniFile, section.pItem, key.pItem);
				if (!iniValue.has_value()) {
					continue;
				}

				auto iniSetting = REX::Ini::CombineSetting(
					static_cast<REX::zstring_view>(section.pItem),
					static_cast<REX::zstring_view>(key.pItem));

				auto scriptKey = RE::BSScript::Variable(RE::BSFixedString(std::move(iniSetting)));
				auto scriptValue = RE::BSScript::Variable(RE::BSFixedString(std::move(iniValue).value()));

				scriptPairs.emplace_back(scriptKey, std::move(scriptValue));
			}
		}

		a_map.SetRange(a_targetNamespace, scriptPairs);
		return true;
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	bool IniFile::UncacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		const auto name = RE::BSFixedString(a_path.lexically_normal().generic_string());
		return a_map.RemoveNamespace(name);
	}

	bool IniFile::SaveCachedAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const
	{
		return SaveCachedAsVariantMapTo(a_map, RE::BSFixedString(a_path.lexically_normal().generic_string()), a_path);
	}

	bool IniFile::SaveCachedAsVariantMapTo(Script::ScriptVariantMap& a_map, const RE::BSFixedString& a_sourceNamespace, const std::filesystem::path& a_targetPath) const
	{
		if (!a_map.ContainsNamespace(a_sourceNamespace)) {
			return false;
		}

		auto iniFile = ini_t();
		SetupFile(iniFile);

		a_map.ForEach(a_sourceNamespace, [&iniFile](const RE::BSScript::Variable& a_key, const RE::BSScript::Variable& a_value) {
			const auto keyString = Impl::GetKeyString(a_key);
			if (!keyString.has_value()) {
				return true;
			}

			const auto scriptValue = Core::IniUtility::FromValue(a_value);
			if (!scriptValue.has_value()) {
				return true;
			}

			const auto&& [sectionName, keyName] = REX::Ini::SplitSetting(keyString.value());

			const auto zeroesSectionName = std::string(sectionName);
			const auto zeroesKeyName = std::string(keyName);

			REX::Ini::SetDetectedValue(iniFile, zeroesSectionName, zeroesKeyName, scriptValue.value());
			return true;
		});

		return WriteFile(a_targetPath, iniFile);
	}
}
