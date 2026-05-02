#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal
{
	SettingsManager::SettingsManager(
		std::filesystem::path a_mainFilePath,
		std::filesystem::path a_customFilePath)
		: _logLevelSetting(std::make_shared<REX::Toml::Int32>("Debug"s, "iLogLevel"s, static_cast<std::int32_t>(REX::LogLevel::kInformation))),
		  _logFileCountSetting(std::make_shared<REX::Toml::Int32>("Debug"s, "iLogFileCount"s, -1)),
		  _logPapyrusMessagesSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogPapyrusMessages"s, false)),
		  _logDebugPapyrusScriptsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogDebugPapyrusScripts"s, false)),
		  _logInvalidLinkedPapyrusScriptsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogInvalidLinkedPapyrusScripts"s, true)),
		  _logInvalidLinkedPapyrusStructsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogInvalidLinkedPapyrusStructs"s, true)),
		  _logDelayedPapyrusFunctionsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogDelayedPapyrusFunctions"s, false)),
		  _logLatentPapyrusFunctionsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogLatentPapyrusFunctions"s, false)),
		  _logDuplicateEditorIdsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bLogDuplicateEditorIds"s, true)),
		  _printCommandRunnerCommandsSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bPrintCommandRunnerCommands"s, false)),
		  _addRequirementRunnerCancelButtonSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bAddRequirementRunnerCancelButton"s, false)),
		  _showBakaFrameworkIncompatibilitiesSetting(std::make_shared<REX::Toml::Bool>("Debug"s, "bShowBakaFrameworkIncompatibilities"s, true)),
		  _scriptTaskletPatch_IncludedScriptNamePatternsSetting(std::make_shared<REX::Toml::StringArray>("Patches"s, "kScriptTaskletPatch_IncludedScriptNamePatterns"s)),
		  _scriptTaskletPatch_ExcludedScriptNamePatternsSetting(std::make_shared<REX::Toml::StringArray>("Patches"s, "kScriptTaskletPatch_ExcludedScriptNamePatterns"s)),
		  _scriptTaskletPatch_IncludedFunctionNamePatternsSetting(std::make_shared<REX::Toml::StringArray>("Patches"s, "kScriptTaskletPatch_IncludedFunctionNamePatterns"s)),
		  _scriptTaskletPatch_ExcludedFunctionNamePatternsSetting(std::make_shared<REX::Toml::StringArray>("Patches"s, "kScriptTaskletPatch_ExcludedFunctionNamePatterns"s)),
		  _settings(std::move(a_mainFilePath), std::move(a_customFilePath))
	{
		const auto settings = std::initializer_list<const REX::NotNull<std::shared_ptr<REX::ISetting>>>{
			_logLevelSetting,
			_logFileCountSetting,
			_logPapyrusMessagesSetting,
			_logDebugPapyrusScriptsSetting,
			_logInvalidLinkedPapyrusScriptsSetting,
			_logInvalidLinkedPapyrusStructsSetting,
			_logDelayedPapyrusFunctionsSetting,
			_logLatentPapyrusFunctionsSetting,
			_logDuplicateEditorIdsSetting,
			_printCommandRunnerCommandsSetting,
			_addRequirementRunnerCancelButtonSetting,
			_showBakaFrameworkIncompatibilitiesSetting,
			_scriptTaskletPatch_IncludedScriptNamePatternsSetting,
			_scriptTaskletPatch_ExcludedScriptNamePatternsSetting,
			_scriptTaskletPatch_IncludedFunctionNamePatternsSetting,
			_scriptTaskletPatch_ExcludedFunctionNamePatternsSetting
		};

		_settings.AddRange(settings);
	}

	SettingsManager::~SettingsManager() noexcept = default;

	void SettingsManager::LoadSettings()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_settings.Load();
	}
}
