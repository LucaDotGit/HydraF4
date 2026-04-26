#include "Plugin/Papyrus/Forms/Spell.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Spell
{
	using SpellFlags = RE::SpellItem::Data::Flags;

	static RE::MagicSystem::SpellType GetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return RE::MagicSystem::SpellType::kNone;
		}

		return a_spell->data.spellType.get_atomic();
	}

	static void SetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		RE::MagicSystem::SpellType a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		a_spell->data.spellType.store_atomic(a_value);
	}

	static RE::MagicSystem::CastingType GetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return RE::MagicSystem::CastingType::kNone;
		}

		return a_spell->data.castingType.get_atomic();
	}

	static void SetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		RE::MagicSystem::CastingType a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		a_spell->data.castingType.store_atomic(a_value);
	}

	static RE::MagicSystem::Delivery GetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return RE::MagicSystem::Delivery::kNone;
		}

		return a_spell->data.delivery.get_atomic();
	}

	static void SetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		RE::MagicSystem::Delivery a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		a_spell->data.delivery.store_atomic(a_value);
	}

	static REX::Float32 GetRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_spell->data.range).load(std::memory_order_acquire);
	}

	static void SetRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		REX::Float32 a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_spell->data.range).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_spell->data.chargeTime).load(std::memory_order_acquire);
	}

	static void SetChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		REX::Float32 a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_spell->data.chargeTime).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetCastDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_spell->data.castDuration).load(std::memory_order_acquire);
	}

	static void SetCastDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		REX::Float32 a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_spell->data.castDuration).store(a_value, std::memory_order_release);
	}

	static RE::BGSPerk* GetCastingPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_spell->data.castingPerk).load(std::memory_order_acquire);
	}

	static void SetCastingPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::SpellItem* a_spell,
		RE::BGSPerk* a_value)
	{
		if (!a_spell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SPELL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_spell->data.castingPerk).store(a_value, std::memory_order_release);
	}

	static SpellFlags GetFlag_HasManualCostCalc(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kManualCostCalc; }
	static SpellFlags GetFlag_IsPlayerStartSpell(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kPCStartSpell; }
	static SpellFlags GetFlag_IsInstantCast(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kInstantCast; }
	static SpellFlags GetFlag_AreaEffectIgnoreLos(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kAreaEffectIgnoresLOS; }
	static SpellFlags GetFlag_IgnoreResist(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kIgnoresResist; }
	static SpellFlags GetFlag_HasNoAbsorbReflect(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kNoAbsorbReflect; }
	static SpellFlags GetFlag_HasNoDualCastMods(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellFlags::kNoDualCastMods; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasManualCostCalc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPlayerStartSpell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsInstantCast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AreaEffectIgnoreLos);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreResist);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoAbsorbReflect);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoDualCastMods);
	}
}