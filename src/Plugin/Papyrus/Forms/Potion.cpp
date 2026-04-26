#include "Plugin/Papyrus/Forms/Potion.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Forms::Potion
{
	using PotionFlags = RE::AlchemyItem::Data::Flags;

	static RE::BGSSoundDescriptorForm* GetConsumeSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_potion->data.consumptionSound).load(std::memory_order_acquire);
	}

	static void SetConsumeSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_potion->data.consumptionSound).store(a_value, std::memory_order_release);
	}

	static RE::SpellItem* GetAddiction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_potion->data.addictionItem).load(std::memory_order_acquire);
	}

	static void SetAddiction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion,
		RE::SpellItem* a_value)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_potion->data.addictionItem).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAddictionChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_potion->data.addictionChance).load(std::memory_order_acquire);
	}

	static void SetAddictionChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion,
		REX::Float32 a_value)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_potion->data.addictionChance).store(a_value, std::memory_order_release);
	}

	static RE::BGSLocalizedString GetAddictionName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return {};
		}

		return a_potion->data.addictionName;
	}

	static void SetAddictionName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::AlchemyItem* a_potion,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_potion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::POTION_NULL, a_stackId);
			return;
		}

		a_potion->data.addictionName = RE::BGSLocalizedString(a_value.GetValue());
	}

	static PotionFlags GetFlag_HasManualCostCalc(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PotionFlags::kManualCostCalc; }
	static PotionFlags GetFlag_IsFood(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PotionFlags::kFood; }
	static PotionFlags GetFlag_IsMedicine(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PotionFlags::kMedicine; }
	static PotionFlags GetFlag_IsPoison(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PotionFlags::kPoison; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConsumeSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConsumeSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAddiction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAddiction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAddictionChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAddictionChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAddictionName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAddictionName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasManualCostCalc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsFood);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsMedicine);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPoison);
	}
}