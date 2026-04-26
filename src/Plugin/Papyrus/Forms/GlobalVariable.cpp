#include "Plugin/Papyrus/Forms/GlobalVariable.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::GlobalVariable
{
	using GlobalRecordFlags = RE::TESGlobal::RecordFlags;

	static REX::Float32 GetValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESGlobal* a_global)
	{
		if (!a_global) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_global->value).load(std::memory_order_acquire);
	}

	static bool SetValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESGlobal* a_global,
		REX::Float32 a_value)
	{
		if (!a_global) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return false;
		}

		if (a_global->formFlags.any_atomic(GlobalRecordFlags::kConstant)) {
			return false;
		}

		std::atomic_ref(a_global->value).store(a_value, std::memory_order_release);
		return true;
	}

	static bool ModValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESGlobal* a_global,
		REX::Float32 a_value)
	{
		if (!a_global) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return false;
		}

		if (a_global->formFlags.any_atomic(GlobalRecordFlags::kConstant)) {
			return false;
		}

		std::atomic_ref(a_global->value).fetch_add(a_value, std::memory_order_acq_rel);
		return true;
	}

	static void ForceSetValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESGlobal* a_global,
		REX::Float32 a_value)
	{
		if (!a_global) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_global->value).store(a_value, std::memory_order_release);
	}

	static void ForceModValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESGlobal* a_global,
		REX::Float32 a_value)
	{
		if (!a_global) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_global->value).fetch_add(a_value, std::memory_order_acq_rel);
	}

	static GlobalRecordFlags GetRecordFlag_IsConstant(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return GlobalRecordFlags::kConstant; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ModValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ForceSetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ForceModValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsConstant);
	}
}
