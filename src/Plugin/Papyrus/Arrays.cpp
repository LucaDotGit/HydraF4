#include "Plugin/Papyrus/Arrays.hpp"

// NOLINTBEGIN(bugprone-suspicious-include)
#include "Plugin/Papyrus/Arrays_Creation.cpp"
#include "Plugin/Papyrus/Arrays_Manipulation.cpp"
#include "Plugin/Papyrus/Arrays_Search.cpp"
// NOLINTEND(bugprone-suspicious-include)

namespace Plugin::Papyrus::Arrays
{
	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		Arrays::Creation::RegisterFunctions(a_vm);
		Arrays::Manipulation::RegisterFunctions(a_vm);
		Arrays::Search::RegisterFunctions(a_vm);
	}
}
