#pragma once

namespace Plugin::Internal::Game
{
	class GameIniSettings;

	class GameGameplaySettings final
	{
	public:
		GameGameplaySettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings);
		~GameGameplaySettings() noexcept;

		GameGameplaySettings(const GameGameplaySettings&) = delete;
		GameGameplaySettings(GameGameplaySettings&&) = delete;

		GameGameplaySettings& operator=(const GameGameplaySettings&) = delete;
		GameGameplaySettings& operator=(GameGameplaySettings&&) = delete;

		void OnGameDataReady();

		[[nodiscard]] RE::DifficultyLevel GetDifficulty() const;
		void SetDifficulty(RE::DifficultyLevel a_value);

	private:
		REX::NotNull<std::shared_ptr<GameIniSettings>> _iniSettings;
		REX::Observer<RE::Setting*> _difficultySetting{ nullptr };
	};
}
