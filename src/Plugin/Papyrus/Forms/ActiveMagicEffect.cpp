#include "Plugin/Papyrus/Forms/ActiveMagicEffect.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::ActiveMagicEffect
{
	using ActiveEffectFlags = RE::ActiveEffect::Flags;

	static std::uint16_t GetId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return 0;
		}

		return a_activeEffect->uniqueID;
	}

	static RE::TESBoundObject* GetSourceObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_activeEffect->source).load(std::memory_order_acquire);
	}

	static RE::ActorHandle GetSourceActor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return a_activeEffect->caster;
	}

	static RE::Actor* GetTargetActor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		auto* target = std::atomic_ref(a_activeEffect->target).load(std::memory_order_acquire);
		if (!target) {
			return nullptr;
		}

		if (!target->MagicTargetIsActor()) {
			return nullptr;
		}

		return RE::DynamicCast<RE::Actor*>(target);
	}

	static RE::EffectSetting* GetBaseEffect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		auto* effect = std::atomic_ref(a_activeEffect->effect).load(std::memory_order_acquire);
		if (!effect) {
			return nullptr;
		}

		return std::atomic_ref(effect->effectSetting).load(std::memory_order_acquire);
	}

	static RE::SpellItem* GetSpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		auto* magicItem = std::atomic_ref(a_activeEffect->spell).load(std::memory_order_acquire);
		if (!magicItem) {
			return nullptr;
		}

		return RE::DynamicCast<RE::SpellItem*>(magicItem);
	}

	static REX::Float32 GetElapsedTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_activeEffect->elapsedSeconds).load(std::memory_order_acquire);
	}

	static REX::Float32 GetDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_activeEffect->duration).load(std::memory_order_acquire);
	}

	static REX::Float32 GetMagnitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_activeEffect->magnitude).load(std::memory_order_acquire);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect,
		ActiveEffectFlags a_flag)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return false;
		}

		return a_activeEffect->flags.any_atomic(a_flag);
	}

	static ActiveEffectFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return ActiveEffectFlags::kNone;
		}

		return a_activeEffect->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect,
		ActiveEffectFlags a_flags)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
		}

		a_activeEffect->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::ActiveEffect> a_activeEffect,
		ActiveEffectFlags a_flag,
		bool a_set)
	{
		if (!a_activeEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVE_EFFECT_NULL, a_stackId);
			return;
		}

		a_activeEffect->flags.set_atomic(a_set, a_flag);
	}

	static ActiveEffectFlags GetFlag_HasNoHitShader(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kNoHitShader; }
	static ActiveEffectFlags GetFlag_HasNoHitEffectArt(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kNoHitEffectArt; }
	static ActiveEffectFlags GetFlag_HasNoInitialFlare(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kNoInitialFlare; }
	static ActiveEffectFlags GetFlag_IsApplyingHitEffects(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kApplyingHitEffects; }
	static ActiveEffectFlags GetFlag_IsApplyingSounds(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kApplyingSounds; }
	static ActiveEffectFlags GetFlag_HasConditions(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kHasConditions; }
	static ActiveEffectFlags GetFlag_Recover(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kRecover; }
	static ActiveEffectFlags GetFlag_IsDualCasted(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kDualCasted; }
	static ActiveEffectFlags GetFlag_IsInactive(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kInactive; }
	static ActiveEffectFlags GetFlag_HasAppliedEffects(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kAppliedEffects; }
	static ActiveEffectFlags GetFlag_HasRemovedEffects(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kRemovedEffects; }
	static ActiveEffectFlags GetFlag_IsDispelled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kDispelled; }
	static ActiveEffectFlags GetFlag_HasWornOff(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActiveEffectFlags::kWornOff; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSourceObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSourceActor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTargetActor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBaseEffect);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetElapsedTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMagnitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoHitShader);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoHitEffectArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoInitialFlare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsApplyingHitEffects);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsApplyingSounds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasConditions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_Recover);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDualCasted);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsInactive);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasAppliedEffects);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasRemovedEffects);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDispelled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasWornOff);
	}
}
