#include "Plugin/Internal/Hooks/ActorValueChangeHook.hpp"

#include "Plugin/Internal/Events/Sources/ActorValueChangeSource.hpp"

namespace Plugin::Internal::Hooks::ActorValueChangeHook::Impl
{
	template <class T>
	concept HookConstraint =
		std::derived_from<T, RE::TESObjectREFR> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;
}

namespace Plugin::Internal::Hooks::ActorValueChangeHook
{
	template <Impl::HookConstraint T>
	class Hook final
	{
	public:
		Hook() = delete;

		static void Setup(REL::HookStore& a_hookStore)
		{
			constexpr const auto& RTTI_ID = T::RTTI;
			constexpr const auto& VTABLE_ID = T::VTABLE[7];

			const auto typeDescriptor = REL::Relocation<const RE::RTTI::TypeDescriptor*>{ RTTI_ID };
			REX::Ensure(static_cast<bool>(typeDescriptor));

			const auto typeDescriptorName = std::string_view(typeDescriptor->name());

			auto hookName01 = REX::Format("{}::ModActorValue"sv, typeDescriptorName);
			auto hookName02 = REX::Format("{}::SetActorValue"sv, typeDescriptorName);

			auto modActorValueHook = std::make_shared<typename decltype(ModActorValueHook)::element_type>(
				std::move(hookName01), VTABLE_ID, 0x06, Hook::ModActorValue);
			auto setActorValueHook = std::make_shared<typename decltype(SetActorValueHook)::element_type>(
				std::move(hookName02), VTABLE_ID, 0x09, Hook::SetActorValue);

			a_hookStore.Add(modActorValueHook);
			a_hookStore.Add(setActorValueHook);

			ModActorValueHook = std::move(modActorValueHook);
			SetActorValueHook = std::move(setActorValueHook);
		}

	private:
		static void ModActorValue(RE::ActorValueOwner* a_self, RE::ACTOR_VALUE_MODIFIER a_modifier, const RE::ActorValueInfo& a_info, REX::Float32 a_value)
		{
			if (!a_self) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			const auto& actorValueChangeEventSource = Events::ActorValueChangeSource::GetEventSource();
			if (!actorValueChangeEventSource->HasSinks()) {
				std::invoke(*ModActorValueHook, a_self, a_modifier, a_info, a_value);
				return;
			}

			const auto oldValue = a_self->GetActorValue(a_info);
			std::invoke(*ModActorValueHook, a_self, a_modifier, a_info, a_value);
			const auto newValue = a_self->GetActorValue(a_info);

			if (oldValue == newValue) {
				return;
			}

			auto* selfRef = RE::DynamicCast<RE::TESObjectREFR*>(a_self);
			if (!selfRef) {
				return;
			}

			auto actorValueChangeEvent = Events::ActorValueChangeEvent{
				.sourceRef = RE::NiPointer(selfRef),
				.sourceValue = const_cast<RE::ActorValueInfo*>(std::addressof(a_info)),
				.oldValue = oldValue,
				.newValue = newValue
			};

			F4SE::GetTaskInterface()->AddTask([&actorValueChangeEventSource, actorValueChangeEvent]() {
				actorValueChangeEventSource->Notify(actorValueChangeEvent);
			});
		}

		static void SetActorValue(RE::ActorValueOwner* a_self, const RE::ActorValueInfo& a_info, REX::Float32 a_value)
		{
			if (!a_self) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			const auto& actorValueChangeEventSource = Events::ActorValueChangeSource::GetEventSource();
			if (!actorValueChangeEventSource->HasSinks()) {
				std::invoke(*SetActorValueHook, a_self, a_info, a_value);
				return;
			}

			auto oldValue = a_self->GetActorValue(a_info);
			std::invoke(*SetActorValueHook, a_self, a_info, a_value);
			auto newValue = a_self->GetActorValue(a_info);

			if (oldValue == newValue) {
				return;
			}

			auto* selfRef = RE::DynamicCast<RE::TESObjectREFR*>(a_self);
			if (!selfRef) {
				return;
			}

			auto actorValueChangeEvent = Events::ActorValueChangeEvent{
				.sourceRef = RE::NiPointer(selfRef),
				.sourceValue = const_cast<RE::ActorValueInfo*>(std::addressof(a_info)),
				.oldValue = oldValue,
				.newValue = newValue
			};

			F4SE::GetTaskInterface()->AddTask([&actorValueChangeEventSource, actorValueChangeEvent]() {
				actorValueChangeEventSource->Notify(actorValueChangeEvent);
			});
		}

		inline static constinit auto ModActorValueHook = std::shared_ptr<REL::HookVft<decltype(&Hook::ModActorValue)>>();
		inline static constinit auto SetActorValueHook = std::shared_ptr<REL::HookVft<decltype(&Hook::SetActorValue)>>();
	};

	void OnXseLoad(REL::HookStore& a_hookStore)
	{
		Hook<RE::TESObjectREFR>::Setup(a_hookStore);
		Hook<RE::Actor>::Setup(a_hookStore);
		Hook<RE::PlayerCharacter>::Setup(a_hookStore);
	}
}
