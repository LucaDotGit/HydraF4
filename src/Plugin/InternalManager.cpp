#include "Plugin/InternalManager.hpp"

#include "Plugin/Internal/EventsManager.hpp"
#include "Plugin/Internal/ExternalManager.hpp"
#include "Plugin/Internal/FunctorsManager.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/GameStartedManager.hpp"
#include "Plugin/Internal/HooksManager.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/PatchesManager.hpp"
#include "Plugin/Internal/RunnersManager.hpp"
#include "Plugin/Internal/SaveManager.hpp"
#include "Plugin/Internal/ScriptManager.hpp"
#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin
{
	static constexpr auto RUNNERS_FILE_EXTENSION = ".json"sv;

	InternalManager::InternalManager()
		: _pluginName(F4SE::PluginVersionData::GetSingleton()->GetPluginName()),
		  _rootDirectoryPath(GetRootDirectoryPath()),
		  _dataDirectoryPath(F4SE::DATA_DIRECTORY_PATH, std::filesystem::path::generic_format),
		  _pluginsDirectoryPath(F4SE::PLUGINS_DIRECTORY_PATH, std::filesystem::path::generic_format),
		  _settingsManager(std::make_shared<Internal::SettingsManager>(
			  std::filesystem::path(_pluginsDirectoryPath / REX::Format("{}.toml"sv, _pluginName), std::filesystem::path::generic_format),
			  std::filesystem::path(_pluginsDirectoryPath / REX::Format("{}_Custom.toml"sv, _pluginName), std::filesystem::path::generic_format))),
		  _hooksManager(std::make_shared<Internal::HooksManager>()),
		  _patchesManager(std::make_shared<Internal::PatchesManager>(_settingsManager)),
		  _functorsManager(std::make_shared<Internal::FunctorsManager>()),
		  _gameManager(std::make_shared<Internal::GameManager>(_settingsManager)),
		  _gameStartedManager(std::make_shared<Internal::GameStartedManager>()),
		  _eventsManager(std::make_shared<Internal::EventsManager>(_gameStartedManager, _gameManager)),
		  _externalManager(std::make_shared<Internal::ExternalManager>(_pluginsDirectoryPath, _settingsManager)),
		  _ioManager(std::make_shared<Internal::IOManager>(_rootDirectoryPath)),
		  _saveManager(std::make_shared<Internal::SaveManager>(_eventsManager)),
		  _scriptManager(std::make_shared<Internal::ScriptManager>(_settingsManager)),
		  _runnersManager(std::make_shared<Internal::RunnersManager>(
			  std::filesystem::path(_dataDirectoryPath / _pluginName, std::filesystem::path::generic_format),
			  std::filesystem::path(RUNNERS_FILE_EXTENSION, std::filesystem::path::generic_format),
			  _settingsManager, _ioManager, _scriptManager, _saveManager))
	{
	}

	InternalManager::~InternalManager() noexcept = default;

	void InternalManager::OnXseLoad()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_hooksManager->OnXseLoad();
		_patchesManager->OnXseLoad();
	}

	void InternalManager::OnPostLoad()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_functorsManager->OnPostLoad();
		_externalManager->OnPostLoad();
	}

	void InternalManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_patchesManager->OnGameDataReady();
		_functorsManager->OnGameDataReady();

		_gameManager->OnGameDataReady();
		_eventsManager->OnGameDataReady();
		_scriptManager->OnGameDataReady();
		_runnersManager->OnGameDataReady();
	}

	void InternalManager::OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_gameStartedManager->OnNewGame(a_charGenQuest);
		_eventsManager->OnNewGame(a_charGenQuest);
	}

	void InternalManager::OnPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_gameStartedManager->OnPostLoadGame(a_isSucceeded);
		_eventsManager->OnPostLoadGame(a_isSucceeded);
	}

	void InternalManager::OnNewOrPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_eventsManager->OnNewOrPostLoadGame(a_isSucceeded);
		_scriptManager->OnNewOrPostLoadGame(a_isSucceeded);
	}

	void InternalManager::OnPostSaveGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_eventsManager->OnPostSaveGame(a_saveName);
	}

	void InternalManager::OnDeleteGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_eventsManager->OnDeleteGame(a_saveName);
	}

	std::filesystem::path InternalManager::GetRootDirectoryPath()
	{
		auto rootDirError = REX::SystemError();
		auto rootDir = std::filesystem::current_path(rootDirError);

		if (rootDirError.value() != REX::ERROR_NUMBER_SUCCESS) [[unlikely]] {
			REX::Fail(
				"Failed to retrieve the application's root directory.\n"
				"System Error (0x{:08X}): {}"sv,
				rootDirError.value(), rootDirError.message());
		}

		if (!rootDir.is_absolute()) [[unlikely]] {
			REX::Fail("Failed to retrieve the application's absolute root directory."sv);
		}

		return { rootDir.lexically_normal().generic_string(), std::filesystem::path::generic_format };
	}

	auto GetInternalManager() -> const REX::NotNull<std::unique_ptr<InternalManager>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<InternalManager>());
		return INSTANCE;
	}
}
