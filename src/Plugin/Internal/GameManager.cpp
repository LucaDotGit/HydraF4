#include "Plugin/Internal/GameManager.hpp"

#include "Plugin/Internal/Game/GameCameraSettings.hpp"
#include "Plugin/Internal/Game/GameEditorIds.hpp"
#include "Plugin/Internal/Game/GameGameplaySettings.hpp"
#include "Plugin/Internal/Game/GameIniSettings.hpp"
#include "Plugin/Internal/Game/GameInternalSettings.hpp"
#include "Plugin/Internal/Game/GameSaveSettings.hpp"
#include "Plugin/Internal/Game/GameUIColors.hpp"
#include "Plugin/Internal/Game/IGameSettingCollection.hpp"

namespace Plugin::Internal
{
	GameManager::GameManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _iniSettings(std::make_shared<Game::GameIniSettings>()),
		  _internalSettings(std::make_shared<Game::GameInternalSettings>()),
		  _cameraSettings(std::make_shared<Game::GameCameraSettings>(_iniSettings)),
		  _editorIds(std::make_shared<Game::GameEditorIds>(a_settingsManager)),
		  _gameplaySettings(std::make_shared<Game::GameGameplaySettings>(_iniSettings)),
		  _saveSettings(std::make_shared<Game::GameSaveSettings>(_iniSettings)),
		  _uiColors(std::make_shared<Game::GameUIColors>(_iniSettings))
	{
		_gameSettingInstances = {
			_iniSettings,
			_internalSettings
		};
	}

	GameManager::~GameManager() noexcept = default;

	void GameManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		LoadGameSettings();

		_cameraSettings->OnGameDataReady();
		_editorIds->OnGameDataReady();
		_gameplaySettings->OnGameDataReady();
		_saveSettings->OnGameDataReady();
		_uiColors->OnGameDataReady();
	}

	void GameManager::LoadGameSettings()
	{
		auto stopwatch = REX::Stopwatch::StartNew();

		{
			std::for_each(std::execution::par, _gameSettingInstances.begin(), _gameSettingInstances.end(),
				[](REX::NotNull<std::shared_ptr<Game::IGameSettingCollection>> a_gameSettingCollection) {
					a_gameSettingCollection->OnGameDataReady();
				});
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Loaded all game settings in {})"sv,
			std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}
}
