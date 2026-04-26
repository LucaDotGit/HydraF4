#include "Plugin/Internal/Game/GameInternalSettings.hpp"

namespace Plugin::Internal::Game
{
	void GameInternalSettings::OnGameDataReady()
	{
		const auto* collection = RE::GameSettingCollection::GetSingleton();
		if (!collection) [[unlikely]] {
			REX::Fail("Failed to get the game's game setting collection."sv);
		}

		for (auto& [key, setting] : collection->settings) {
			if (!setting) {
				continue;
			}

			_settings.emplace(key, setting);
		}
	}

	auto GameInternalSettings::GetKeys() const -> std::vector<RE::BSFixedString>
	{
		auto keys = std::vector<RE::BSFixedString>();
		keys.reserve(_settings.size());

		for (const auto& [key, _] : _settings) {
			keys.push_back(key);
		}

		return keys;
	}

	auto GameInternalSettings::GetSortedKeys() const -> std::vector<RE::BSFixedString>
	{
		auto keys = GetKeys();
		std::ranges::sort(keys);
		return keys;
	}

	bool GameInternalSettings::ContainsSetting(std::string_view a_key) const
	{
		const auto key = RE::BSFixedString(a_key);
		return ContainsSetting(key);
	}

	bool GameInternalSettings::ContainsSetting(const RE::BSFixedString& a_key) const
	{
		return _settings.contains(a_key);
	}

	REX::Observer<RE::Setting*> GameInternalSettings::GetSetting(std::string_view a_key) const
	{
		const auto key = RE::BSFixedString(a_key);
		return GetSetting(key);
	}

	REX::Observer<RE::Setting*> GameInternalSettings::GetSetting(const RE::BSFixedString& a_key) const
	{
		const auto settingIt = _settings.find(a_key);
		if (settingIt == _settings.end()) {
			return nullptr;
		}

		return settingIt->second.get();
	}
}
