#include "Plugin/Internal/RunnersManager.hpp"

#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/SaveManager.hpp"
#include "Plugin/Internal/ScriptManager.hpp"

#include "Plugin/Internal/Runners/ConsoleCommandRunner.hpp"
#include "Plugin/Internal/Runners/MenuRunner.hpp"
#include "Plugin/Internal/Runners/MiscStatRunner.hpp"
#include "Plugin/Internal/Runners/RequirementRunner.hpp"
#include "Plugin/Internal/Runners/ScriptFileCacheRunner.hpp"
#include "Plugin/Internal/Runners/ScriptFunctionRunner.hpp"
#include "Plugin/Internal/Runners/ScriptObjectRunner.hpp"
#include "Plugin/Internal/Runners/ScriptReflectionRunner.hpp"
#include "Plugin/Internal/Runners/ScriptSaveMapRunner.hpp"
#include "Plugin/Internal/Runners/ScriptSaveSetRunner.hpp"
#include "Plugin/Internal/Runners/ScriptStrictStringRunner.hpp"
#include "Plugin/Internal/Runners/TranslationRunner.hpp"

namespace Plugin::Internal
{
	static constexpr auto COMMANDS_DIRECTORY_NAME = "ConsoleCommands"sv;
	static constexpr auto MENUS_DIRECTORY_NAME = "Menus"sv;
	static constexpr auto MISC_STATS_DIRECTORY_NAME = "MiscStats"sv;
	static constexpr auto REQUIREMENTS_DIRECTORY_NAME = "Requirements"sv;
	static constexpr auto SCRIPT_FILE_CACHE_DIRECTORY_NAME = "ScriptFileCaches"sv;
	static constexpr auto SCRIPT_FUNCTIONS_DIRECTORY_NAME = "ScriptFunctions"sv;
	static constexpr auto SCRIPT_OBJECTS_DIRECTORY_NAME = "ScriptObjects"sv;
	static constexpr auto SCRIPT_REFLECTIONS_DIRECTORY_NAME = "ScriptReflections"sv;
	static constexpr auto SCRIPT_SAVE_MAPS_DIRECTORY_NAME = "ScriptSaveMaps"sv;
	static constexpr auto SCRIPT_SAVE_SETS_DIRECTORY_NAME = "ScriptSaveSets"sv;
	static constexpr auto SCRIPT_STRICT_STRINGS_DIRECTORY_NAME = "ScriptStrictStrings"sv;
	static constexpr auto TRANSLATIONS_DIRECTORY_NAME = "Translations"sv;

	RunnersManager::RunnersManager(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager,
		const REX::NotNull<std::shared_ptr<IOManager>>& a_ioManager,
		const REX::NotNull<std::shared_ptr<ScriptManager>>& a_scriptManager,
		const REX::NotNull<std::shared_ptr<SaveManager>>& a_saveManager)
		: _directoryPath(std::move(a_directoryPath)),
		  _fileExtension(std::move(a_fileExtension)),
		  _consoleCommandRunner(std::make_shared<Runners::ConsoleCommandRunner>(
			  std::filesystem::path(_directoryPath / COMMANDS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_settingsManager)),
		  _menuRunner(std::make_shared<Runners::MenuRunner>(
			  std::filesystem::path(_directoryPath / MENUS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension)),
		  _miscStatRunner(std::make_shared<Runners::MiscStatRunner>(
			  std::filesystem::path(_directoryPath / MISC_STATS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension)),
		  _requirementRunner(std::make_shared<Runners::RequirementRunner>(
			  std::filesystem::path(_directoryPath / REQUIREMENTS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_settingsManager)),
		  _scriptFileCacheRunner(std::make_shared<Runners::ScriptFileCacheRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_FILE_CACHE_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_ioManager->GetIOSandbox(), a_scriptManager->GetMemoryMap())),
		  _scriptFunctionRunner(std::make_shared<Runners::ScriptFunctionRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_FUNCTIONS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_saveManager->GetScriptFunctionMap())),
		  _scriptObjectRunner(std::make_shared<Runners::ScriptObjectRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_OBJECTS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_saveManager->GetScriptObjectMap())),
		  _scriptReflectionRunner(std::make_shared<Runners::ScriptReflectionRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_REFLECTIONS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension)),
		  _scriptSaveMapRunner(std::make_shared<Runners::ScriptSaveMapRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_SAVE_MAPS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_saveManager->GetScriptVariantMap())),
		  _scriptSaveSetRunner(std::make_shared<Runners::ScriptSaveSetRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_SAVE_SETS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension, a_saveManager->GetScriptVariantSet())),
		  _scriptStrictStringRunner(std::make_shared<Runners::ScriptStrictStringRunner>(
			  std::filesystem::path(_directoryPath / SCRIPT_STRICT_STRINGS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension)),
		  _translationRunner(std::make_shared<Runners::TranslationRunner>(
			  std::filesystem::path(_directoryPath / TRANSLATIONS_DIRECTORY_NAME, std::filesystem::path::generic_format),
			  _fileExtension))
	{
		_runnerInstances = {
			_consoleCommandRunner,
			_menuRunner,
			_miscStatRunner,
			_requirementRunner,
			_scriptFileCacheRunner,
			_scriptFunctionRunner,
			_scriptObjectRunner,
			_scriptReflectionRunner,
			_scriptSaveMapRunner,
			_scriptSaveSetRunner,
			_scriptStrictStringRunner,
			_translationRunner
		};
	}

	RunnersManager::~RunnersManager() noexcept = default;

	void RunnersManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		ExecuteRunners();
	}

	void RunnersManager::ExecuteRunners()
	{
		auto stopwatch = REX::Stopwatch::StartNew();

		{
			auto threadPool = REX::ThreadPool();

			{
				auto threadSetupResults = threadPool.SubmitTaskRange(
					_runnerInstances, [](const REX::NotNull<std::shared_ptr<Runners::IFileRunner>>& a_runnerInstance, std::reference_wrapper<REX::ThreadPool> a_threadPool) {
						return a_runnerInstance->Setup(a_threadPool.get());
					},
					std::ref(threadPool));

				threadPool.AttachThisUntilDrained();

				for (auto& threadSetupResult : threadSetupResults) {
					const auto threadResults = threadSetupResult.get();

					for (const auto& threadResult : threadResults) {
						threadResult.wait();
					}
				}
			}

			{
				auto threadRunResults = threadPool.SubmitTaskRange(
					_runnerInstances, [](const REX::NotNull<std::shared_ptr<Runners::IFileRunner>>& a_runnerInstance, std::reference_wrapper<REX::ThreadPool> a_threadPool) {
						return a_runnerInstance->Run(a_threadPool.get());
					},
					std::ref(threadPool));

				threadPool.AttachThisUntilDrained();

				for (auto& threadRunResult : threadRunResults) {
					const auto threadResults = threadRunResult.get();

					for (const auto& threadResult : threadResults) {
						threadResult.wait();
					}
				}
			}

			{
				const auto threadDisposeResults = threadPool.SubmitTaskRange(
					_runnerInstances, [](const REX::NotNull<std::shared_ptr<Runners::IFileRunner>>& a_runnerInstance) {
						a_runnerInstance->Dispose();
					});

				threadPool.AttachThisUntilDrained();

				for (const auto& threadDisposeResult : threadDisposeResults) {
					threadDisposeResult.wait();
				}
			}
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Executed all runners in {})"sv,
			std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}
}
