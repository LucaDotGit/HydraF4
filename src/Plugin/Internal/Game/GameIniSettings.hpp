#pragma once

#include "Plugin/Internal/Game/IGameSettingCollection.hpp"

namespace Plugin::Internal::Game
{
	class GameIniSettings final
		: public IGameSettingCollection
	{
	public:
		GameIniSettings() = default;
		~GameIniSettings() noexcept override = default;

		GameIniSettings(const GameIniSettings&) = delete;
		GameIniSettings(GameIniSettings&&) = delete;

		GameIniSettings& operator=(const GameIniSettings&) = delete;
		GameIniSettings& operator=(GameIniSettings&&) = delete;

		void OnGameDataReady() override;

		[[nodiscard]] auto GetSections() const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] auto GetSortedSections() const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] auto GetSettings() const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] auto GetSortedSettings() const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] auto GetKeys(std::string_view a_section) const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] auto GetSortedKeys(std::string_view a_section) const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] auto GetKeys(const RE::BSFixedString& a_section) const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] auto GetSortedKeys(const RE::BSFixedString& a_section) const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] bool ContainsSetting(std::string_view a_section, std::string_view a_key) const;
		[[nodiscard]] bool ContainsSetting(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const;

		[[nodiscard]] bool ContainsSetting(std::string_view a_setting) const;
		[[nodiscard]] bool ContainsSetting(const RE::BSFixedString& a_setting) const;

		[[nodiscard]] bool ContainsSection(std::string_view a_section) const;
		[[nodiscard]] bool ContainsSection(const RE::BSFixedString& a_section) const;

		[[nodiscard]] bool ContainsKey(std::string_view a_section, std::string_view a_key) const;
		[[nodiscard]] bool ContainsKey(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const;

		[[nodiscard]] auto GetSetting(std::string_view a_section, std::string_view a_key) const -> REX::Observer<RE::Setting*>;
		[[nodiscard]] auto GetSetting(const RE::BSFixedString& a_section, const RE::BSFixedString& a_key) const -> REX::Observer<RE::Setting*>;

		[[nodiscard]] auto GetSetting(std::string_view a_setting) const -> REX::Observer<RE::Setting*>;
		[[nodiscard]] auto GetSetting(const RE::BSFixedString& a_setting) const -> REX::Observer<RE::Setting*>;

		static bool SaveSetting(REX::Observer<RE::Setting*> a_setting);
		static bool SaveSettingAsync(REX::Observer<RE::Setting*> a_setting);

	private:
		std::unordered_map<RE::BSFixedString, std::unordered_map<RE::BSFixedString, REX::NotNull<REX::Observer<RE::Setting*>>>> _pairSettings;
		std::unordered_map<RE::BSFixedString, REX::NotNull<REX::Observer<RE::Setting*>>> _flatSettings;
	};
}
