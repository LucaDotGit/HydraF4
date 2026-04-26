#include "Plugin/Papyrus/StringPairs.hpp"

#include "Plugin/Internal/Structs/StringPairs.hpp"

namespace Plugin::Papyrus::StringPairs
{
	using StringPairStruct = ::Plugin::Internal::Structs::StringPairs::StringPairData;

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<StringPairStruct> a_left,
		std::optional<StringPairStruct> a_right)
	{
		return REX::GetNumericPartialOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<StringPairStruct> a_left,
		std::optional<StringPairStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
