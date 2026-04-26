#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
	class IOManager;
	class ScriptManager;
	class SaveManager;
}

namespace Plugin::Internal::Runners
{
	class IFileRunner;
	class ConsoleCommandRunner;
	class MenuRunner;
	class MiscStatRunner;
	class RequirementRunner;
	class ScriptFileCacheRunner;
	class ScriptFunctionRunner;
	class ScriptObjectRunner;
	class ScriptReflectionRunner;
	class ScriptSaveMapRunner;
	class ScriptSaveSetRunner;
	class ScriptStrictStringRunner;
	class TranslationRunner;
}

namespace Plugin::Internal
{
	class RunnersManager final
	{
	public:
		RunnersManager(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager,
			const REX::NotNull<std::shared_ptr<IOManager>>& a_ioManager,
			const REX::NotNull<std::shared_ptr<ScriptManager>>& a_scriptManager,
			const REX::NotNull<std::shared_ptr<SaveManager>>& a_saveManager);

		~RunnersManager() noexcept;

		RunnersManager(const RunnersManager&) = delete;
		RunnersManager(RunnersManager&&) = delete;

		RunnersManager& operator=(const RunnersManager&) = delete;
		RunnersManager& operator=(RunnersManager&&) = delete;

		[[nodiscard]] const auto& GetDirectoryPath() const noexcept { return _directoryPath; }
		[[nodiscard]] const auto& GetFileExtension() const noexcept { return _fileExtension; }

		[[nodiscard]] auto GetConsoleCommandRunner() const noexcept { return _consoleCommandRunner; }
		[[nodiscard]] auto GetMenuRunner() const noexcept { return _menuRunner; }
		[[nodiscard]] auto GetMiscStatRunner() const noexcept { return _miscStatRunner; }
		[[nodiscard]] auto GetRequirementRunner() const noexcept { return _requirementRunner; }
		[[nodiscard]] auto GetScriptFileCacheRunner() const noexcept { return _scriptFileCacheRunner; }
		[[nodiscard]] auto GetScriptFunctionRunner() const noexcept { return _scriptFunctionRunner; }
		[[nodiscard]] auto GetScriptReflectionRunner() const noexcept { return _scriptReflectionRunner; }
		[[nodiscard]] auto GetScriptSaveMapRunner() const noexcept { return _scriptSaveMapRunner; }
		[[nodiscard]] auto GetScriptSaveSetRunner() const noexcept { return _scriptSaveSetRunner; }
		[[nodiscard]] auto GetScriptObjectRunner() const noexcept { return _scriptObjectRunner; }
		[[nodiscard]] auto GetScriptStrictStringRunner() const noexcept { return _scriptStrictStringRunner; }
		[[nodiscard]] auto GetTranslationRunner() const noexcept { return _translationRunner; }

		[[nodiscard]] auto GetRunnerInstances() const noexcept { return std::span(_runnerInstances); }

		void OnGameDataReady();

	private:
		void ExecuteRunners();

		std::filesystem::path _directoryPath;
		std::filesystem::path _fileExtension;
		REX::NotNull<std::shared_ptr<Runners::ConsoleCommandRunner>> _consoleCommandRunner;
		REX::NotNull<std::shared_ptr<Runners::MenuRunner>> _menuRunner;
		REX::NotNull<std::shared_ptr<Runners::MiscStatRunner>> _miscStatRunner;
		REX::NotNull<std::shared_ptr<Runners::RequirementRunner>> _requirementRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptFileCacheRunner>> _scriptFileCacheRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptFunctionRunner>> _scriptFunctionRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptObjectRunner>> _scriptObjectRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptReflectionRunner>> _scriptReflectionRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptSaveMapRunner>> _scriptSaveMapRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptSaveSetRunner>> _scriptSaveSetRunner;
		REX::NotNull<std::shared_ptr<Runners::ScriptStrictStringRunner>> _scriptStrictStringRunner;
		REX::NotNull<std::shared_ptr<Runners::TranslationRunner>> _translationRunner;
		std::vector<REX::NotNull<std::shared_ptr<Runners::IFileRunner>>> _runnerInstances;
	};
}
