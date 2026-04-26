#include "Plugin/Papyrus/Forms/Enchantment.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Enchantment
{
	using EnchantmentFlags = RE::EnchantmentItem::Data::Flags;

	static RE::MagicSystem::SpellType GetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return RE::MagicSystem::SpellType::kNone;
		}

		return a_enchantment->data.spellType.get_atomic();
	}

	static void SetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		RE::MagicSystem::SpellType a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		a_enchantment->data.spellType.store_atomic(a_value);
	}

	static RE::MagicSystem::CastingType GetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return RE::MagicSystem::CastingType::kNone;
		}

		return a_enchantment->data.castingType.get_atomic();
	}

	static void SetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		RE::MagicSystem::CastingType a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		a_enchantment->data.castingType.store_atomic(a_value);
	}

	static RE::MagicSystem::Delivery GetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return RE::MagicSystem::Delivery::kNone;
		}

		return a_enchantment->data.delivery.get_atomic();
	}

	static void SetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		RE::MagicSystem::Delivery a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		a_enchantment->data.delivery.store_atomic(a_value);
	}

	static REX::Float32 GetChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_enchantment->data.chargeTime).load(std::memory_order_acquire);
	}

	static void SetChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		REX::Float32 a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_enchantment->data.chargeTime).store(a_value, std::memory_order_release);
	}

	static RE::EnchantmentItem* GetBaseEnchantment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_enchantment->data.baseEnchantment).load(std::memory_order_acquire);
	}

	static void SetBaseEnchantment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		RE::EnchantmentItem* a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_enchantment->data.baseEnchantment).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetWornRestrictionList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_enchantment->data.wornRestrictions).load(std::memory_order_acquire);
	}

	static void SetWornRestrictionList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EnchantmentItem* a_enchantment,
		RE::BGSListForm* a_value)
	{
		if (!a_enchantment) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCHANTMENT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_enchantment->data.wornRestrictions).store(a_value, std::memory_order_release);
	}

	static EnchantmentFlags GetFlag_HasManualCostCalc(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EnchantmentFlags::kManualCostCalc; }
	static EnchantmentFlags GetFlag_IsFood(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EnchantmentFlags::kFoodItem; }
	static EnchantmentFlags GetFlag_HasExtendedDuration(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EnchantmentFlags::kExtendDuration; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBaseEnchantment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBaseEnchantment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWornRestrictionList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWornRestrictionList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasManualCostCalc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsFood);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasExtendedDuration);
	}
}