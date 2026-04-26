#include "Plugin/Internal/Game/GameIniSettings.hpp"

namespace Plugin::Internal::Game
{
	static constexpr auto FALLBACK_SECTION = "MAIN"sv;

	void GameIniSettings::OnGameDataReady()
	{
		const auto* settings = RE::INISettingCollection::GetSingleton();
		if (!settings) [[unlikely]] {
			REX::Fail("Failed to get the game's INI setting collection."sv);
		}

		const auto* prefSettings = RE::INIPrefSettingCollection::GetSingleton();
		if (!prefSettings) [[unlikely]] {
			REX::Fail("Failed to get the game's INI pref setting collection."sv);
		}

		const auto addSettings = [this](const RE::SettingCollectionList<RE::Setting>* a_settingCollection) {
			for (auto* setting : a_settingCollection->settings) {
				if (!setting) {
					continue;
				}

				const auto oldKey = std::string_view(setting->GetKey());
				if (oldKey.empty()) {
					continue;
				}

				const auto separatorIndex = oldKey.find(REX::Ini::SETTING_SEPARATOR);
				const auto hasSection = separatorIndex != std::string::npos;

				const auto zeroedSection = std::string(hasSection ? oldKey.substr(separatorIndex + 1) : FALLBACK_SECTION);
				const auto zeroedKey = std::string(hasSection ? oldKey.substr(0, separatorIndex) : oldKey);
				const auto combinedSetting = REX::Ini::CombineSetting(zeroedSection, zeroedKey);

				_pairSettings[RE::BSFixedString(zeroedSection)].insert_or_assign(RE::BSFixedString(zeroedKey), setting);
				_flatSettings.insert_or_assign(RE::BSFixedString(combinedSetting), setting);
			}
		};

		addSettings(settings);
		addSettings(prefSettings);
	}

	auto GameIniSettings::GetSections() const -> std::vector<RE::BSFixedString>
	{
		auto sections = std::vector<RE::BSFixedString>();
		sections.reserve(_pairSettings.size());

		for (const auto& [section, _] : _pairSettings) {
			sections.push_back(section);
		}

		return sections;
	}

	auto GameIniSettings::GetSortedSections() const -> std::vector<RE::BSFixedString>
	{
		auto sections = GetSections();
		std::ranges::sort(sections);
		return sections;
	}

	auto GameIniSettings::GetSettings() const -> std::vector<RE::BSFixedString>
	{
		auto settings = std::vector<RE::BSFixedString>();
		settings.reserve(_flatSettings.size());

		for (const auto& [key, _] : _flatSettings) {
			settings.push_back(key);
		}

		return settings;
	}

	auto GameIniSettings::GetSortedSettings() const -> std::vector<RE::BSFixedString>
	{
		auto settings = GetSettings();
		std::ranges::sort(settings);
		return settings;
	}

	auto GameIniSettings::GetKeys(std::string_view a_section) const -> std::vector<RE::BSFixedString>
	{
		const auto section = RE::BSFixedString(a_section);
		return GetKeys(section);
	}

	auto GameIniSettings::GetSortedKeys(std::string_view a_section) const -> std::vector<RE::BSFixedString>
	{
		const auto section = RE::BSFixedString(a_section);
		auto keys = GetKeys(section);
		std::ranges::sort(keys);
		return keys;
	}

	auto GameIniSettings::GetKeys(const RE::BSFixedString& a_section) const -> std::vector<RE::BSFixedString>
	{
		const auto sectionIt = _pairSettings.find(a_section);
		if (sectionIt == _pairSettings.end()) {
			return {};
		}

		auto keys = std::vector<RE::BSFixedString>();
		keys.reserve(sectionIt->second.size());

		for (const auto& [key, _] : sectionIt->second) {
			keys.push_back(key);
		}

		return keys;
	}

	auto GameIniSettings::GetSortedKeys(const RE::BSFixedString& a_section) const -> std::vector<RE::BSFixedString>
	{
		auto keys = GetKeys(a_section);
		std::ranges::sort(keys);
		return keys;
	}

	bool GameIniSettings::ContainsSetting(std::string_view a_section, std::string_view a_key) const
	{
		const auto section = RE::BSFixedString(a_section);
		const auto key = RE::BSFixedString(a_key);

		return ContainsSetting(section, key);
	}

	bool GameIniSettings::ContainsSetting(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const
	{
		const auto sectionIt = _pairSettings.find(a_section);
		if (sectionIt == _pairSettings.end()) {
			return false;
		}

		return sectionIt->second.contains(a_key);
	}

	bool GameIniSettings::ContainsSetting(std::string_view a_setting) const
	{
		const auto setting = RE::BSFixedString(a_setting);
		return ContainsSetting(setting);
	}

	bool GameIniSettings::ContainsSetting(const RE::BSFixedString& a_setting) const
	{
		return _flatSettings.contains(a_setting);
	}

	bool GameIniSettings::ContainsSection(std::string_view a_section) const
	{
		const auto section = RE::BSFixedString(a_section);
		return ContainsSection(section);
	}

	bool GameIniSettings::ContainsSection(const RE::BSFixedString& a_section) const
	{
		return _pairSettings.contains(a_section);
	}

	bool GameIniSettings::ContainsKey(std::string_view a_section, std::string_view a_key) const
	{
		const auto section = RE::BSFixedString(a_section);
		const auto key = RE::BSFixedString(a_key);

		return ContainsKey(section, key);
	}

	bool GameIniSettings::ContainsKey(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const
	{
		const auto sectionIt = _pairSettings.find(a_section);
		if (sectionIt == _pairSettings.end()) {
			return false;
		}

		return sectionIt->second.contains(a_key);
	}

	auto GameIniSettings::GetSetting(std::string_view a_section, std::string_view a_key) const -> REX::Observer<RE::Setting*>
	{
		const auto section = RE::BSFixedString(a_section);
		const auto key = RE::BSFixedString(a_key);

		return GetSetting(section, key);
	}

	auto GameIniSettings::GetSetting(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const -> REX::Observer<RE::Setting*>
	{
		const auto sectionIt = _pairSettings.find(a_section);
		if (sectionIt == _pairSettings.end()) {
			return nullptr;
		}

		const auto keyIt = sectionIt->second.find(a_key);
		if (keyIt == sectionIt->second.end()) {
			return nullptr;
		}

		return keyIt->second.get();
	}

	auto GameIniSettings::GetSetting(std::string_view a_setting) const -> REX::Observer<RE::Setting*>
	{
		const auto setting = RE::BSFixedString(a_setting);
		return GetSetting(setting);
	}

	auto GameIniSettings::GetSetting(const RE::BSFixedString& a_setting) const -> REX::Observer<RE::Setting*>
	{
		const auto settingIt = _flatSettings.find(a_setting);
		if (settingIt == _flatSettings.end()) {
			return nullptr;
		}

		return settingIt->second.get();
	}

	bool GameIniSettings::SaveSetting(REX::Observer<RE::Setting*> a_setting)
	{
		if (!a_setting) {
			return false;
		}

		auto* settings =
			a_setting->IsPrefSetting() ?
				RE::INIPrefSettingCollection::GetSingleton() :
				RE::INISettingCollection::GetSingleton();

		if (!settings) [[unlikely]] {
			REX::Fail("Failed to get the game's INI setting collection."sv);
		}

		return settings->WriteSetting(*a_setting);
	}

	bool GameIniSettings::SaveSettingAsync(REX::Observer<RE::Setting*> a_setting)
	{
		if (!a_setting) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([a_setting]() {
			SaveSetting(a_setting);
		});

		return true;
	}
}
