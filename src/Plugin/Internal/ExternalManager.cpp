#include "Plugin/Internal/ExternalManager.hpp"

#include "Plugin/Internal/External/BakaFrameworkPatch.hpp"

namespace Plugin::Internal
{
	ExternalManager::ExternalManager(
		const std::filesystem::path& a_pluginsDirectoryPath,
		const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _bakaFrameworkPatch(std::make_shared<External::BakaFrameworkPatch>(a_pluginsDirectoryPath, a_settingsManager))
	{
	}

	ExternalManager::~ExternalManager() noexcept = default;

	void ExternalManager::OnPostLoad()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_bakaFrameworkPatch->OnPostLoad();
	}
}
