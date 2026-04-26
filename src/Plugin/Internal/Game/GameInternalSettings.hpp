#pragma once

#include "Plugin/Internal/Game/IGameSettingCollection.hpp"

namespace Plugin::Internal::Game
{
	class GameInternalSettings final
		: public IGameSettingCollection
	{
	public:
		GameInternalSettings() = default;
		~GameInternalSettings() noexcept override = default;

		GameInternalSettings(const GameInternalSettings&) = delete;
		GameInternalSettings(GameInternalSettings&&) = delete;

		GameInternalSettings& operator=(const GameInternalSettings&) = delete;
		GameInternalSettings& operator=(GameInternalSettings&&) = delete;

		void OnGameDataReady() override;

		[[nodiscard]] auto GetKeys() const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] auto GetSortedKeys() const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] bool ContainsSetting(std::string_view a_key) const;
		[[nodiscard]] bool ContainsSetting(const RE::BSFixedString& a_key) const;

		[[nodiscard]] auto GetSetting(std::string_view a_key) const -> REX::Observer<RE::Setting*>;
		[[nodiscard]] auto GetSetting(const RE::BSFixedString& a_key) const -> REX::Observer<RE::Setting*>;

	private:
		std::unordered_map<RE::BSFixedString, REX::NotNull<REX::Observer<RE::Setting*>>> _settings;
	};
}
