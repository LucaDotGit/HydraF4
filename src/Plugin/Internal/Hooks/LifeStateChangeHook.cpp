#include "Plugin/Internal/Hooks/LifeStateChangeHook.hpp"

#include "Plugin/Internal/Events/Sources/LifeStateChangeSource.hpp"

namespace Plugin::Internal::Hooks::LifeStateChangeHook::Impl
{
	template <class T>
	concept HookConstraint =
		std::derived_from<T, RE::Actor> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;

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

			auto hookName = REX::Format("{}::SetLifeState"sv, typeDescriptorName);

			auto setLifeStateHook = std::make_shared<typename decltype(SetLifeStateHook)::element_type>(
				std::move(hookName), VTABLE_ID, 0x105, Hook::SetLifeState);

			a_hookStore.Add(setLifeStateHook);

			SetLifeStateHook = std::move(setLifeStateHook);
		}

	private:
		static void SetLifeState(RE::Actor* a_self, RE::ACTOR_LIFE_STATE a_lifeState)
		{
			if (!a_self) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			const auto oldState = a_self->lifeState;
			std::invoke(*SetLifeStateHook, a_self, a_lifeState);

			if (oldState == a_lifeState) {
				return;
			}

			const auto& lifeStateChangeEventSource = Events::LifeStateChangeSource::GetEventSource();
			if (!lifeStateChangeEventSource->HasSinks()) {
				return;
			}

			auto lifeStateChangeEvent = Events::LifeStateChangeEvent{
				.actor = RE::NiPointer(a_self),
				.oldState = oldState,
				.newState = a_lifeState
			};

			F4SE::GetTaskInterface()->AddTask([&lifeStateChangeEventSource, lifeStateChangeEvent]() {
				lifeStateChangeEventSource->Notify(lifeStateChangeEvent);
			});
		}

		inline static constinit auto SetLifeStateHook = std::shared_ptr<REL::HookVft<decltype(&Hook::SetLifeState)>>();
	};
}

namespace Plugin::Internal::Hooks::LifeStateChangeHook
{
	void OnXseLoad(REL::HookStore& a_hookStore)
	{
		Impl::Hook<RE::Actor>::Setup(a_hookStore);
		Impl::Hook<RE::PlayerCharacter>::Setup(a_hookStore);
	}
}
