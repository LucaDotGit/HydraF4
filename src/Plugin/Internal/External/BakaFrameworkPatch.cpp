#include "Plugin/Internal/External/BakaFrameworkPatch.hpp"

#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::External
{
	static constexpr auto BAKA_FRAMEWORK_PLUGIN_NAME = "BakaFramework"sv;
	static constexpr auto BAKA_FRAMEWORK_NEW_VERSION = REX::Version(2, 0, 0, 0);

	BakaFrameworkPatch::BakaFrameworkPatch(
		const std::filesystem::path& a_pluginsDirectoryPath,
		const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _settingsManager(a_settingsManager),
		  _oldEnableLoadingEditorIds(std::make_shared<REX::Toml::Bool>("Patches"s, "EnableLoadingEditorIDs"s, false)),
		  _newEnableLoadingEditorIds(std::make_shared<REX::Ini::Bool>("Patches"s, "bEnableLoadingEditorIDs"s, false)),
		  _oldSettings(
			  std::filesystem::path(a_pluginsDirectoryPath / REX::Format("{}.toml"sv, BAKA_FRAMEWORK_PLUGIN_NAME), std::filesystem::path::generic_format)),
		  _newSettings(
			  std::filesystem::path(a_pluginsDirectoryPath / REX::Format("{}.ini"sv, BAKA_FRAMEWORK_PLUGIN_NAME), std::filesystem::path::generic_format),
			  std::filesystem::path(a_pluginsDirectoryPath / REX::Format("{}Custom.ini"sv, BAKA_FRAMEWORK_PLUGIN_NAME), std::filesystem::path::generic_format))
	{
		_oldSettings.Add(_oldEnableLoadingEditorIds);
		_newSettings.Add(_newEnableLoadingEditorIds);
	}

	BakaFrameworkPatch::~BakaFrameworkPatch() noexcept = default;

	void BakaFrameworkPatch::OnPostLoad()
	{
		if (!_settingsManager->GetShowBakaFrameworkIncompatibilitiesSetting()->GetValue()) {
			return;
		}

		const auto* plugin = F4SE::GetPluginInfo(BAKA_FRAMEWORK_PLUGIN_NAME.data());
		if (!plugin) {
			return;
		}

		const auto pluginVersion = plugin->GetPluginVersion();
		if (ShowOldError(pluginVersion)) {
			return;
		}

		ShowNewError(pluginVersion);
	}

	bool BakaFrameworkPatch::ShowOldError(REX::Version a_pluginVersion)
	{
		if (a_pluginVersion >= BAKA_FRAMEWORK_NEW_VERSION) {
			return false;
		}

		_oldSettings.Load();

		const auto enableLoadingEditorIds = _oldEnableLoadingEditorIds->GetValue();
		if (!enableLoadingEditorIds) {
			return true;
		}

		const auto mainFilePath = _oldSettings.GetMainPath().generic_string();
		const auto selfPluginName = F4SE::GetPluginName();

		REX::Fail(
			"Baka Framework has been detected with its editor ID patch enabled.\n"
			"Please disable the setting \"{}:{}\" in \"{}\", as {} supersedes this patch."sv,
			_oldEnableLoadingEditorIds->GetSection(), _oldEnableLoadingEditorIds->GetKey(), mainFilePath, selfPluginName);

		return true;
	}

	bool BakaFrameworkPatch::ShowNewError(REX::Version a_pluginVersion)
	{
		if (a_pluginVersion < BAKA_FRAMEWORK_NEW_VERSION) {
			return false;
		}

		_newSettings.Load();

		const auto enableLoadingEditorIds = _newEnableLoadingEditorIds->GetValue();
		if (!enableLoadingEditorIds) {
			return true;
		}

		const auto mainFilePath = _newSettings.GetMainPath().generic_string();
		const auto customFilePath = _newSettings.GetCustomPath().generic_string();
		const auto selfPluginName = F4SE::GetPluginName();

		REX::Fail(
			"Baka Framework has been detected with its editor ID patch enabled.\n"
			"Please disable the setting \"{}:{}\" in \"{}\" (and \"{}\", if it exists), as {} supersedes this patch."sv,
			_newEnableLoadingEditorIds->GetSection(), _newEnableLoadingEditorIds->GetKey(), mainFilePath, customFilePath, selfPluginName);

		return true;
	}
}
