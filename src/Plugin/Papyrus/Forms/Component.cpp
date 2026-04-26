#include "Plugin/Papyrus/Forms/Component.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Component
{
	static RE::TESObjectMISC* GetScrapItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSComponent* a_component)
	{
		if (!a_component) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COMPONENT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_component->scrapItem).load(std::memory_order_acquire);
	}

	static void SetScrapItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSComponent* a_component,
		RE::TESObjectMISC* a_value)
	{
		if (!a_component) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COMPONENT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_component->scrapItem).store(a_value, std::memory_order_release);
	}

	static RE::TESGlobal* GetScrapScalar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSComponent* a_component)
	{
		if (!a_component) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COMPONENT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_component->modScrapScalar).load(std::memory_order_acquire);
	}

	static void SetScrapScalar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSComponent* a_component,
		RE::TESGlobal* a_value)
	{
		if (!a_component) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COMPONENT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_component->modScrapScalar).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetScrapItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetScrapItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetScrapScalar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetScrapScalar);
	}
}