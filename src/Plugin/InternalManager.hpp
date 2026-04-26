#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
	class HooksManager;
	class PatchesManager;
	class FunctorsManager;
	class GameManager;
	class GameStartedManager;
	class EventsManager;
	class ExternalManager;
	class IOManager;
	class SaveManager;
	class ScriptManager;
	class RunnersManager;
}

namespace Plugin
{
	class InternalManager final
	{
	public:
		InternalManager();
		~InternalManager() noexcept;

		InternalManager(const InternalManager&) = delete;
		InternalManager(InternalManager&&) = delete;

		InternalManager& operator=(const InternalManager&) = delete;
		InternalManager& operator=(InternalManager&&) = delete;

		[[nodiscard]] auto GetSettingsManager() const noexcept { return _settingsManager; }
		[[nodiscard]] auto GetHooksManager() const noexcept { return _hooksManager; }
		[[nodiscard]] auto GetPatchesManager() const noexcept { return _patchesManager; }
		[[nodiscard]] auto GetFunctorsManager() const noexcept { return _functorsManager; }
		[[nodiscard]] auto GetGameManager() const noexcept { return _gameManager; }
		[[nodiscard]] auto GetGameStartedManager() const noexcept { return _gameStartedManager; }
		[[nodiscard]] auto GetEventsManager() const noexcept { return _eventsManager; }
		[[nodiscard]] auto GetExternalManager() const noexcept { return _externalManager; }
		[[nodiscard]] auto GetIOManager() const noexcept { return _ioManager; }
		[[nodiscard]] auto GetSaveManager() const noexcept { return _saveManager; }
		[[nodiscard]] auto GetScriptManager() const noexcept { return _scriptManager; }
		[[nodiscard]] auto GetRunnersManager() const noexcept { return _runnersManager; }

		void OnXseLoad();
		void OnPostLoad();
		void OnGameDataReady();
		void OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest);
		void OnPostLoadGame(bool a_isSucceeded);
		void OnNewOrPostLoadGame(bool a_isSucceeded);
		void OnPostSaveGame(const RE::BSFixedString& a_saveName);
		void OnDeleteGame(const RE::BSFixedString& a_saveName);

	private:
		[[nodiscard]] static std::filesystem::path GetRootDirectoryPath();

		std::string_view _pluginName;
		std::filesystem::path _rootDirectoryPath;
		std::filesystem::path _dataDirectoryPath;
		std::filesystem::path _pluginsDirectoryPath;
		REX::NotNull<std::shared_ptr<Internal::SettingsManager>> _settingsManager;
		REX::NotNull<std::shared_ptr<Internal::HooksManager>> _hooksManager;
		REX::NotNull<std::shared_ptr<Internal::PatchesManager>> _patchesManager;
		REX::NotNull<std::shared_ptr<Internal::FunctorsManager>> _functorsManager;
		REX::NotNull<std::shared_ptr<Internal::GameManager>> _gameManager;
		REX::NotNull<std::shared_ptr<Internal::GameStartedManager>> _gameStartedManager;
		REX::NotNull<std::shared_ptr<Internal::EventsManager>> _eventsManager;
		REX::NotNull<std::shared_ptr<Internal::ExternalManager>> _externalManager;
		REX::NotNull<std::shared_ptr<Internal::IOManager>> _ioManager;
		REX::NotNull<std::shared_ptr<Internal::SaveManager>> _saveManager;
		REX::NotNull<std::shared_ptr<Internal::ScriptManager>> _scriptManager;
		REX::NotNull<std::shared_ptr<Internal::RunnersManager>> _runnersManager;
	};

	[[nodiscard]] auto GetInternalManager() -> const REX::NotNull<std::unique_ptr<InternalManager>>&;
}
