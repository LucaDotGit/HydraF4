#include "Plugin/PluginManager.hpp"

#include "Plugin/Internal/SettingsManager.hpp"
#include "Plugin/InternalManager.hpp"
#include "Plugin/MessagingManager.hpp"
#include "Plugin/PapyrusManager.hpp"
#include "Plugin/ScaleformManager.hpp"
#include "Plugin/SerializationManager.hpp"

namespace Plugin::PluginManager
{
	bool Setup(REX::NotNull<const F4SE::LoadInterface*> a_f4se)
	{
		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [a_f4se]() {
			const auto& internalManager = Plugin::GetInternalManager();

			const auto settingsManager = internalManager->GetSettingsManager();
			settingsManager->LoadSettings();

			const auto logLevel = static_cast<REX::LogLevel>(settingsManager->GetLogLevelSetting()->GetValue());
			const auto logFileCount = static_cast<std::size_t>(settingsManager->GetLogFileCountSetting()->GetValue());

			const auto initInfo = F4SE::InitInfo{
				.logLevel = logLevel,
				.logFileCount = logFileCount,
				.useHooks = true,
				.useTrampoline = true
			};

			F4SE::RegisterForOnLoad([&internalManager]() {
				internalManager->OnXseLoad();
			});

			F4SE::Init(a_f4se, initInfo);

			if (settingsManager->GetLogPapyrusMessagesSetting()->GetValue()) {
				F4SE::Events::RegisterForScriptLogForwarding();
			}

			Plugin::MessagingManager::Setup(F4SE::GetMessagingInterface());
			Plugin::PapyrusManager::Setup(F4SE::GetPapyrusInterface());
			Plugin::ScaleformManager::Setup(F4SE::GetScaleformInterface());
			Plugin::SerializationManager::Setup(F4SE::GetSerializationInterface());
		});

		return true;
	}
}
