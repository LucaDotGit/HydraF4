#include "Plugin/Papyrus/Strings.hpp"

// NOLINTBEGIN(bugprone-suspicious-include)
#include "Plugin/Papyrus/Strings_Manipulation.cpp"
#include "Plugin/Papyrus/Strings_Search.cpp"
// NOLINTEND(bugprone-suspicious-include)

namespace Plugin::Papyrus::Strings
{
	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		Strings::Search::RegisterFunctions(a_vm);
		Strings::Manipulation::RegisterFunctions(a_vm);
	}
}
