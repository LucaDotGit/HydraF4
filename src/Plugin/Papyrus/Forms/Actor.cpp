#include "Plugin/Papyrus/Forms/Actor.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Actor
{
	static RE::TESNPC* GetActorBase(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return nullptr;
		}

		auto* baseObject = std::atomic_ref(a_actor->data.objectReference).load(std::memory_order_acquire);
		if (!baseObject) {
			return nullptr;
		}

		return RE::DynamicCast<RE::TESNPC*>(baseObject);
	}

	static void SetActorBase(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::TESNPC* a_value)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return;
		}

		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actor->SetObjectReference(a_value);
	}

	static bool ContainsEffect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::EffectSetting* a_effect)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return false;
		}

		if (!a_effect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return false;
		}

		const auto* activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			return false;
		}

		return activeEffects->ForEachActiveEffect([a_effect](const RE::BSTSmartPointer<RE::ActiveEffect>& a_activeEffect) {
			if (!a_activeEffect) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			auto* effectItem = a_activeEffect->effect;
			if (!effectItem) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			auto* effectSetting = effectItem->effectSetting;
			if (effectSetting != a_effect) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			return RE::BSContainer::ForEachResult::kStop;
		}) == RE::BSContainer::ForEachResult::kStop;
	}

	static std::optional<std::vector<RE::EffectSetting*>> GetEffects(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			return std::nullopt;
		}

		auto result = std::vector<RE::EffectSetting*>();
		activeEffects->ForEachActiveEffect([&result](const RE::BSTSmartPointer<RE::ActiveEffect>& a_activeEffect) {
			if (!a_activeEffect) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			auto* effectItem = a_activeEffect->effect;
			if (!effectItem) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			auto* effectSetting = effectItem->effectSetting;
			if (effectSetting) {
				result.push_back(effectSetting);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static bool ContainsActiveEffect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::ActiveEffect* a_activeEffect)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return false;
		}

		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return false;
		}

		const auto* activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			return false;
		}

		return activeEffects->ForEachActiveEffect([a_activeEffect](const RE::BSTSmartPointer<RE::ActiveEffect>& a_iterActiveEffect) {
			if (a_iterActiveEffect.get() != a_activeEffect) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			return RE::BSContainer::ForEachResult::kStop;
		}) == RE::BSContainer::ForEachResult::kStop;
	}

	static std::optional<std::vector<RE::BSTSmartPointer<RE::ActiveEffect>>> GetActiveEffects(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return std::nullopt;
		}

		auto* activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			return std::nullopt;
		}

		auto result = std::vector<RE::BSTSmartPointer<RE::ActiveEffect>>();
		activeEffects->ForEachActiveEffect([&result](const RE::BSTSmartPointer<RE::ActiveEffect>& a_activeEffect) {
			if (a_activeEffect) {
				result.push_back(a_activeEffect);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static bool ContainsSpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::SpellItem* a_spell)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return false;
		}

		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return false;
		}

		return std::ranges::contains(a_actor->addedSpells, a_spell);
	}

	static std::vector<RE::SpellItem*> GetSpells(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return {};
		}

		auto result = std::vector<RE::SpellItem*>();
		for (auto* spell : a_actor->addedSpells) {
			if (spell) {
				result.push_back(spell);
			}
		}

		return result;
	}

	static bool CanUseIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::TESIdleForm* a_idle)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return false;
		}

		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return false;
		}

		return a_actor->CanUseIdle(a_idle);
	}

	static RE::TESIdleForm* GetCurrentIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return nullptr;
		}

		const auto* currentProcess = a_actor->currentProcess;
		if (!currentProcess) {
			return nullptr;
		}

		const auto* middleHigh = currentProcess->middleHigh;
		if (!middleHigh) {
			return nullptr;
		}

		return middleHigh->currentIdle;
	}

	static RE::TESIdleForm* GetCurrentFurnitureIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return nullptr;
		}

		const auto* currentProcess = a_actor->currentProcess;
		if (!currentProcess) {
			return nullptr;
		}

		const auto* middleHigh = currentProcess->middleHigh;
		if (!middleHigh) {
			return nullptr;
		}

		return middleHigh->furnitureIdle;
	}

	static RE::TESIdleForm* GetLastPlayedIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return nullptr;
		}

		const auto* currentProcess = a_actor->currentProcess;
		if (!currentProcess) {
			return nullptr;
		}

		const auto* middleHigh = currentProcess->middleHigh;
		if (!middleHigh) {
			return nullptr;
		}

		return middleHigh->lastIdlePlayed;
	}

	static std::uint8_t GetPerkRank(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::BGSPerk* a_perk)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return 0;
		}

		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return 0;
		}

		return a_actor->GetPerkRank(a_perk);
	}

	static void Update3DAsync(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor,
		RE::RESET_3D_FLAGS a_flags)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return;
		}

		static_assert(RE::RESET_3D_FLAGS::kNone < RE::RESET_3D_FLAGS::kAll);
		auto excludedFlags = REX::EnumSet(RE::RESET_3D_FLAGS::kAll);

		if (a_flags < RE::RESET_3D_FLAGS::kNone || a_flags > RE::RESET_3D_FLAGS::kAll) [[unlikely]] {
			excludedFlags.store(RE::RESET_3D_FLAGS::kNone);
		}
		else {
			excludedFlags.reset(a_flags);
		}

		F4SE::GetTaskInterface()->AddTask([actorRef = RE::NiPointer(a_actor), excludedFlags]() {
			constexpr auto RELOAD_ALL = false;
			constexpr auto QUEUE_RESET = false;
			constexpr auto ADDITIONAL_FLAGS = REX::EnumSet(RE::RESET_3D_FLAGS::kNone);

			actorRef->Reset3D(RELOAD_ALL, ADDITIONAL_FLAGS.get(), QUEUE_RESET, excludedFlags.get());
		});
	}

	static RE::SIT_SLEEP_STATE GetSitSleepState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto SIT_SLEEP_STATE_NONE = static_cast<RE::SIT_SLEEP_STATE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return SIT_SLEEP_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return SIT_SLEEP_STATE_NONE;
		}

		return std::atomic_ref(actorState->sitSleepState).load(std::memory_order_acquire);
	}

	static RE::FLY_STATE GetFlyState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return RE::FLY_STATE::kNone;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return RE::FLY_STATE::kNone;
		}

		return std::atomic_ref(actorState->flyState).load(std::memory_order_acquire);
	}

	static RE::ACTOR_LIFE_STATE GetLifeState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto ACTOR_LIFE_STATE_NONE = static_cast<RE::ACTOR_LIFE_STATE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return ACTOR_LIFE_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return ACTOR_LIFE_STATE_NONE;
		}

		return std::atomic_ref(actorState->lifeState).load(std::memory_order_acquire);
	}

	static RE::ATTACK_STATE_ENUM GetAttackState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return RE::ATTACK_STATE_ENUM::kNone;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return RE::ATTACK_STATE_ENUM::kNone;
		}

		return std::atomic_ref(actorState->attackState).load(std::memory_order_acquire);
	}

	static RE::KNOCK_STATE_ENUM GetKnockState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto KNOCK_STATE_NONE = static_cast<RE::KNOCK_STATE_ENUM>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return KNOCK_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return KNOCK_STATE_NONE;
		}

		return std::atomic_ref(actorState->knockState).load(std::memory_order_acquire);
	}

	static RE::WEAPON_STATE GetWeaponState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto WEAPON_STATE_NONE = static_cast<RE::WEAPON_STATE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return WEAPON_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return WEAPON_STATE_NONE;
		}

		return std::atomic_ref(actorState->weaponState).load(std::memory_order_acquire);
	}

	static RE::RECOIL_ENUM GetRecoilState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return RE::RECOIL_ENUM::kNone;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return RE::RECOIL_ENUM::kNone;
		}

		return std::atomic_ref(actorState->recoilState).load(std::memory_order_acquire);
	}

	static RE::ACTOR_STANCE GetStanceState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto ACTOR_STANCE_NONE = static_cast<RE::ACTOR_STANCE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return ACTOR_STANCE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return ACTOR_STANCE_NONE;
		}

		return std::atomic_ref(actorState->stance).load(std::memory_order_acquire);
	}

	static RE::GUN_STATE GetGunState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto GUN_STATE_NONE = static_cast<RE::GUN_STATE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return GUN_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return GUN_STATE_NONE;
		}

		return std::atomic_ref(actorState->gunState).load(std::memory_order_acquire);
	}

	static RE::INTERACTING_STATE GetInteractionState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::Actor* a_actor)
	{
		constexpr auto INTERACTING_STATE_NONE = static_cast<RE::INTERACTING_STATE>(-1);

		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return INTERACTING_STATE_NONE;
		}

		const auto* actorState = DynamicCast<const RE::ActorState*>(a_actor);
		if (!actorState) {
			return INTERACTING_STATE_NONE;
		}

		return std::atomic_ref(actorState->interactingState).load(std::memory_order_acquire);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActorBase);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetActorBase);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsEffect);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEffects);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsActiveEffect);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActiveEffects);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsSpell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSpells);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, CanUseIdle);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentIdle);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentFurnitureIdle);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLastPlayedIdle);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPerkRank);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Update3DAsync);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSitSleepState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlyState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLifeState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAttackState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKnockState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWeaponState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetStanceState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetGunState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInteractionState);
	}
}
