#include "Plugin/Papyrus/Forms/DamageType.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::DamageType
{
	static RE::ActorValueInfo* GetResistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSDamageType* a_damageType)
	{
		if (!a_damageType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DAMAGE_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_damageType->data.resistance).load(std::memory_order_acquire);
	}

	static void SetResistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSDamageType* a_damageType,
		RE::ActorValueInfo* a_value)
	{
		if (!a_damageType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DAMAGE_TYPE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_damageType->data.resistance).store(a_value, std::memory_order_release);
	}

	static RE::SpellItem* GetSpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSDamageType* a_damageType)
	{
		if (!a_damageType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DAMAGE_TYPE_NULL, a_stackId);
			return nullptr;
		}

		auto* spell = std::atomic_ref(a_damageType->data.spell).load(std::memory_order_acquire);
		if (!spell) {
			return nullptr;
		}

		return RE::DynamicCast<RE::SpellItem*>(spell);
	}

	static void SetSpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSDamageType* a_damageType,
		RE::SpellItem* a_value)
	{
		if (!a_damageType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DAMAGE_TYPE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_damageType->data.spell).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetResistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetResistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpell);
	}
}
