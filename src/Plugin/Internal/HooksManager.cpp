#include "Plugin/Internal/HooksManager.hpp"

#include "Plugin/Internal/Hooks/ActorValueChangeHook.hpp"
#include "Plugin/Internal/Hooks/AnimationGraphEventHook.hpp"
#include "Plugin/Internal/Hooks/ButtonUpDownHook.hpp"
#include "Plugin/Internal/Hooks/LifeStateChangeHook.hpp"

namespace Plugin::Internal
{
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void HooksManager::OnXseLoad()
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, []() noexcept {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			const auto& hookStore = REL::GetHookStore();

			Hooks::ActorValueChangeHook::OnXseLoad(*(hookStore.get()));
			Hooks::AnimationGraphEventHook::OnXseLoad(*(hookStore.get()));
			Hooks::ButtonUpDownHook::OnXseLoad(*(hookStore.get()));
			Hooks::LifeStateChangeHook::OnXseLoad(*(hookStore.get()));
		});
	}
}
