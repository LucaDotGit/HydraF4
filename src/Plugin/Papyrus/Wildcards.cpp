#include "Plugin/Papyrus/Wildcards.hpp"

namespace Plugin::Papyrus::Wildcards
{
	static bool IsMatch(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		std::string_view a_pattern)
	{
		return REX::MatchWildcardsIgnoreCase(a_string, a_pattern);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsMatch);
	}
}
