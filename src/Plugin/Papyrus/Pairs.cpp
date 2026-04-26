#include "Plugin/Papyrus/Pairs.hpp"

#include "Plugin/Internal/Structs/Pairs.hpp"

namespace Plugin::Papyrus::Pairs
{
	using PairStruct = ::Plugin::Internal::Structs::Pairs::PairData;

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<PairStruct> a_left,
		std::optional<PairStruct> a_right)
	{
		return REX::GetNumericPartialOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<PairStruct> a_left,
		std::optional<PairStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
