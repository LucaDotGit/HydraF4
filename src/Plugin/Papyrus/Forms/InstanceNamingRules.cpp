#include "Plugin/Papyrus/Forms/InstanceNamingRules.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::InstanceNamingRules
{
	static RE::FormType GetTargetFormType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSInstanceNamingRules* a_namingRules)
	{
		if (!a_namingRules) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INSTANCE_NAMING_RULE_NULL, a_stackId);
			return RE::FormType::kNone;
		}

		return a_namingRules->type.get_atomic();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTargetFormType);
	}
}
