#pragma once

namespace Plugin::Internal
{
	class SettingsManager final
	{
	public:
		SettingsManager(
			std::filesystem::path a_mainFilePath,
			std::filesystem::path a_customFilePath);

		~SettingsManager() noexcept;

		SettingsManager(const SettingsManager&) = delete;
		SettingsManager(SettingsManager&&) = delete;

		SettingsManager& operator=(const SettingsManager&) = delete;
		SettingsManager& operator=(SettingsManager&&) = delete;

		[[nodiscard]] const auto& GetFilePath() const noexcept { return _settings.GetMainPath(); }
		[[nodiscard]] const auto& GetCustomFilePath() const noexcept { return _settings.GetCustomPath(); }

		[[nodiscard]] auto GetLogLevelSetting() const noexcept { return _logLevelSetting; }
		[[nodiscard]] auto GetLogFileCountSetting() const noexcept { return _logFileCountSetting; }
		[[nodiscard]] auto GetLogPapyrusMessagesSetting() const noexcept { return _logPapyrusMessagesSetting; }
		[[nodiscard]] auto GetLogDebugPapyrusScriptsSetting() const noexcept { return _logDebugPapyrusScriptsSetting; }
		[[nodiscard]] auto GetLogInvalidLinkedPapyrusScriptsSetting() const noexcept { return _logInvalidLinkedPapyrusScriptsSetting; }
		[[nodiscard]] auto GetLogInvalidLinkedPapyrusStructsSetting() const noexcept { return _logInvalidLinkedPapyrusStructsSetting; }
		[[nodiscard]] auto GetLogDelayedPapyrusFunctionsSetting() const noexcept { return _logDelayedPapyrusFunctionsSetting; }
		[[nodiscard]] auto GetLogLatentPapyrusFunctionsSetting() const noexcept { return _logLatentPapyrusFunctionsSetting; }
		[[nodiscard]] auto GetLogDuplicateEditorIdsSetting() const noexcept { return _logDuplicateEditorIdsSetting; }
		[[nodiscard]] auto GetPrintCommandRunnerCommandsSetting() const noexcept { return _printCommandRunnerCommandsSetting; }
		[[nodiscard]] auto GetAddRequirementRunnerCancelButtonSetting() const noexcept { return _addRequirementRunnerCancelButtonSetting; }
		[[nodiscard]] auto GetShowBakaFrameworkIncompatibilitiesSetting() const noexcept { return _showBakaFrameworkIncompatibilitiesSetting; }
		[[nodiscard]] auto GetScriptTaskletPatch_IncludedScriptNamePatternsSetting() const noexcept { return _scriptTaskletPatch_IncludedScriptNamePatternsSetting; }
		[[nodiscard]] auto GetScriptTaskletPatch_ExcludedScriptNamePatternsSetting() const noexcept { return _scriptTaskletPatch_ExcludedScriptNamePatternsSetting; }
		[[nodiscard]] auto GetScriptTaskletPatch_IncludedFunctionNamePatternsSetting() const noexcept { return _scriptTaskletPatch_IncludedFunctionNamePatternsSetting; }
		[[nodiscard]] auto GetScriptTaskletPatch_ExcludedFunctionNamePatternsSetting() const noexcept { return _scriptTaskletPatch_ExcludedFunctionNamePatternsSetting; }

		void LoadSettings();

	private:
		REX::NotNull<std::shared_ptr<REX::Toml::Int32>> _logLevelSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Int32>> _logFileCountSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logPapyrusMessagesSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logDebugPapyrusScriptsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logInvalidLinkedPapyrusScriptsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logInvalidLinkedPapyrusStructsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logDelayedPapyrusFunctionsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logLatentPapyrusFunctionsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _logDuplicateEditorIdsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _printCommandRunnerCommandsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _addRequirementRunnerCancelButtonSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _showBakaFrameworkIncompatibilitiesSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::StringArray>> _scriptTaskletPatch_IncludedScriptNamePatternsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::StringArray>> _scriptTaskletPatch_ExcludedScriptNamePatternsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::StringArray>> _scriptTaskletPatch_IncludedFunctionNamePatternsSetting;
		REX::NotNull<std::shared_ptr<REX::Toml::StringArray>> _scriptTaskletPatch_ExcludedFunctionNamePatternsSetting;
		REX::Toml::SettingStore _settings;
	};
}
