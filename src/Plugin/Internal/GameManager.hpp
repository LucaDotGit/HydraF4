#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Game
{
	class IGameSettingCollection;
	class GameCameraSettings;
	class GameEditorIds;
	class GameGameplaySettings;
	class GameIniSettings;
	class GameInternalSettings;
	class GameSaveSettings;
	class GameUIColors;
}

namespace Plugin::Internal
{
	class GameManager final
	{
	public:
		GameManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~GameManager() noexcept;

		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;

		GameManager& operator=(const GameManager&) = delete;
		GameManager& operator=(GameManager&&) = delete;

		[[nodiscard]] auto GetIniSettings() const noexcept { return _iniSettings; }
		[[nodiscard]] auto GetInternalSettings() const noexcept { return _internalSettings; }
		[[nodiscard]] auto GetCameraSettings() const noexcept { return _cameraSettings; }
		[[nodiscard]] auto GetEditorIds() const noexcept { return _editorIds; }
		[[nodiscard]] auto GetGameplaySettings() const noexcept { return _gameplaySettings; }
		[[nodiscard]] auto GetSaveSettings() const noexcept { return _saveSettings; }
		[[nodiscard]] auto GetUIColors() const noexcept { return _uiColors; }

		void OnGameDataReady();

	private:
		void LoadGameSettings();

		REX::NotNull<std::shared_ptr<Game::GameIniSettings>> _iniSettings;
		REX::NotNull<std::shared_ptr<Game::GameInternalSettings>> _internalSettings;
		REX::NotNull<std::shared_ptr<Game::GameCameraSettings>> _cameraSettings;
		REX::NotNull<std::shared_ptr<Game::GameEditorIds>> _editorIds;
		REX::NotNull<std::shared_ptr<Game::GameGameplaySettings>> _gameplaySettings;
		REX::NotNull<std::shared_ptr<Game::GameSaveSettings>> _saveSettings;
		REX::NotNull<std::shared_ptr<Game::GameUIColors>> _uiColors;
		std::vector<REX::NotNull<std::shared_ptr<Game::IGameSettingCollection>>> _gameSettingInstances;
	};
}
