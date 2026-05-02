#include "Plugin/Internal/Hooks/AnimationGraphEventHook.hpp"

#include "Plugin/Internal/Events/Sources/AnimationGraphEventSource.hpp"

namespace Plugin::Internal::Hooks::AnimationGraphEventHook::Impl
{
	template <class T>
	concept HookConstraint =
		std::derived_from<T, RE::TESObjectREFR> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;

	[[nodiscard]] static RE::TESObjectREFR* DynamicCast(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_self)
	{
		if (!a_self) {
			return nullptr;
		}

		static const auto FROM_RTTI = REL::Relocation<void*>{ RE::RTTI::BSTEventSink_BSAnimationGraphEvent_ };
		static const auto TO_RTTI = REL::Relocation<void*>{ RE::RTTI::TESObjectREFR };

		return static_cast<RE::TESObjectREFR*>(
			RE::RTDynamicCast(static_cast<void*>(a_self),
				0,
				FROM_RTTI.get(),
				TO_RTTI.get(),
				0));
	}

	template <Impl::HookConstraint T>
	class Hook final
	{
	public:
		Hook() = delete;

		static void Setup(REL::HookStore& a_hookStore)
		{
			constexpr const auto& RTTI_ID = T::RTTI;
			constexpr const auto& VTABLE_ID = T::VTABLE[3];

			const auto typeDescriptor = REL::Relocation<const RE::RTTI::TypeDescriptor*>{ RTTI_ID };
			REX::Ensure(static_cast<bool>(typeDescriptor));

			const auto typeDescriptorName = std::string_view(typeDescriptor->name());

			auto hookName = REX::Format("{}::ProcessEvent"sv, typeDescriptorName);

			auto processEventHook = std::make_shared<typename decltype(ProcessEventHook)::element_type>(
				std::move(hookName), VTABLE_ID, 0x01, Hook::ProcessEvent);

			a_hookStore.Add(processEventHook);

			ProcessEventHook = std::move(processEventHook);
		}

	private:
		static RE::BSEventNotifyControl ProcessEvent(
			RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_self,
			const RE::BSAnimationGraphEvent& a_event,
			RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			if (!a_self) [[unlikely]] {
				REX::Assert(false);
				return RE::BSEventNotifyControl::kStop;
			}

			if (std::invoke(*ProcessEventHook, a_self, a_event, a_eventSource) == RE::BSEventNotifyControl::kStop) {
				return RE::BSEventNotifyControl::kStop;
			}

			const auto& animationGraphEventSource = Events::AnimationGraphEventSource::GetEventSource();
			if (!animationGraphEventSource->HasSinks()) {
				return RE::BSEventNotifyControl::kContinue;
			}

			auto* selfRef = Impl::DynamicCast(a_self);
			if (!selfRef) [[unlikely]] {
				REX::Assert(false);
				return RE::BSEventNotifyControl::kContinue;
			}

			auto animationGraphEvent = Events::AnimationGraphEvent{
				.sourceRef = RE::NiPointer(selfRef),
				.eventName = a_event.tag,
				.payload = a_event.payload
			};

			F4SE::GetTaskInterface()->AddTask([&animationGraphEventSource, animationGraphEvent = std::move(animationGraphEvent)]() {
				animationGraphEventSource->Notify(animationGraphEvent);
			});

			return RE::BSEventNotifyControl::kContinue;
		}

		inline static constinit auto ProcessEventHook = std::shared_ptr<REL::HookVft<decltype(&Hook::ProcessEvent)>>();
	};
}

namespace Plugin::Internal::Hooks::AnimationGraphEventHook
{
	void OnXseLoad(REL::HookStore& a_hookStore)
	{
		Impl::Hook<RE::TESObjectREFR>::Setup(a_hookStore);
		Impl::Hook<RE::Actor>::Setup(a_hookStore);
		Impl::Hook<RE::PlayerCharacter>::Setup(a_hookStore);
	}
}