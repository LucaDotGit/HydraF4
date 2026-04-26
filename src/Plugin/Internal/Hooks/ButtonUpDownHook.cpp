#include "Plugin/Internal/Hooks/ButtonUpDownHook.hpp"

#include "Plugin/Internal/Events/Sources/ButtonUpDownSource.hpp"

namespace Plugin::Internal::Hooks::ButtonUpDownHook::Impl
{
	template <class T>
	concept HookConstraint =
		std::derived_from<T, RE::BSInputEventReceiver> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;
}

namespace Plugin::Internal::Hooks::ButtonUpDownHook
{
	template <Impl::HookConstraint T>
	class Hook final
	{
	public:
		Hook() = delete;

		static void Setup(REL::HookStore& a_hookStore)
		{
			constexpr const auto& RTTI_ID = T::RTTI;
			constexpr const auto& VTABLE_ID = T::VTABLE[0];

			const auto typeDescriptor = REL::Relocation<const RE::RTTI::TypeDescriptor*>{ RTTI_ID };
			REX::Ensure(static_cast<bool>(typeDescriptor));

			const auto typeDescriptorName = std::string_view(typeDescriptor->name());

			auto hookName = REX::Format("{}::PerformInputProcessing"sv, typeDescriptorName);

			auto performInputProcessingHook = std::make_shared<typename decltype(PerformInputProcessingHook)::element_type>(
				std::move(hookName), VTABLE_ID, 0x00, Hook::PerformInputProcessing);

			a_hookStore.Add(performInputProcessingHook);

			PerformInputProcessingHook = std::move(performInputProcessingHook);
		}

	private:
		static void PerformInputProcessing(RE::MenuControls* a_self, const RE::InputEvent* a_queueHead)
		{
			if (!a_self) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			std::invoke(*PerformInputProcessingHook, a_self, a_queueHead);

			if (!a_queueHead) {
				return;
			}

			const auto* eventInfo = a_queueHead->As<RE::ButtonEvent>();
			if (!eventInfo || eventInfo->disabled) {
				return;
			}

			const auto& buttonUpDownEventSource = Events::ButtonUpDownSource::GetEventSource();
			if (!buttonUpDownEventSource->HasSinks()) {
				return;
			}

			auto buttonUpDownEvent = Events::ButtonUpDownEvent{
				.deviceType = eventInfo->device,
				.buttonCode = eventInfo->GetBSButtonCode(),
				.controlName = eventInfo->strUserEvent,
				.analogValue = eventInfo->value,
				.heldSeconds = eventInfo->heldDownSecs
			};

			F4SE::GetTaskInterface()->AddTask([&buttonUpDownEventSource, buttonUpDownEvent = std::move(buttonUpDownEvent)]() {
				buttonUpDownEventSource->Notify(buttonUpDownEvent);
			});
		}

		inline static constinit auto PerformInputProcessingHook = std::shared_ptr<REL::HookVft<decltype(&Hook::PerformInputProcessing)>>();
	};

	void OnXseLoad(REL::HookStore& a_hookStore)
	{
		Hook<RE::MenuControls>::Setup(a_hookStore);
	}
}