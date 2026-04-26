#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::External
{
	class BakaFrameworkPatch final
	{
	public:
		BakaFrameworkPatch(
			const std::filesystem::path& a_pluginsDirectoryPath,
			const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);

		~BakaFrameworkPatch() noexcept;

		BakaFrameworkPatch(const BakaFrameworkPatch&) = delete;
		BakaFrameworkPatch(BakaFrameworkPatch&&) = delete;

		BakaFrameworkPatch& operator=(const BakaFrameworkPatch&) = delete;
		BakaFrameworkPatch& operator=(BakaFrameworkPatch&&) = delete;

		[[nodiscard]] const auto& GetOldFilePath() const noexcept { return _oldSettings.GetMainPath(); }
		[[nodiscard]] const auto& GetNewFilePath() const noexcept { return _newSettings.GetMainPath(); }
		[[nodiscard]] const auto& GetOldCustomFilePath() const noexcept { return _oldSettings.GetCustomPath(); }
		[[nodiscard]] const auto& GetNewCustomFilePath() const noexcept { return _newSettings.GetCustomPath(); }

		[[nodiscard]] auto GetOldEnableLoadingEditorIds() const noexcept { return _oldEnableLoadingEditorIds; }
		[[nodiscard]] auto GetEnableLoadingEditorIds() const noexcept { return _newEnableLoadingEditorIds; }

		void OnPostLoad();

	private:
		bool ShowOldError(REX::Version a_pluginVersion);
		bool ShowNewError(REX::Version a_pluginVersion);

		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
		REX::NotNull<std::shared_ptr<REX::Toml::Bool>> _oldEnableLoadingEditorIds;
		REX::NotNull<std::shared_ptr<REX::Ini::Bool>> _newEnableLoadingEditorIds;
		REX::Toml::SettingStore _oldSettings;
		REX::Ini::SettingStore _newSettings;
	};
}
