#pragma once

namespace Plugin::Internal::External
{
	class BakaFrameworkPatch;
}

namespace Plugin::Internal
{
	class SettingsManager;

	class ExternalManager final
	{
	public:
		ExternalManager(
			const std::filesystem::path& a_pluginsDirectoryPath,
			const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);

		~ExternalManager() noexcept;

		ExternalManager(const ExternalManager&) = delete;
		ExternalManager(ExternalManager&&) = delete;

		ExternalManager& operator=(const ExternalManager&) = delete;
		ExternalManager& operator=(ExternalManager&&) = delete;

		void OnPostLoad();

	private:
		REX::NotNull<std::shared_ptr<External::BakaFrameworkPatch>> _bakaFrameworkPatch;
	};
}
