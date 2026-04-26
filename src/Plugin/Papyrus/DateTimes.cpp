#include "Plugin/Papyrus/DateTimes.hpp"

#include "Plugin/Internal/Structs/DateTimes.hpp"

namespace Plugin::Papyrus::DateTimes
{
	using DateTimeStruct = ::Plugin::Internal::Structs::DateTimes::DateTimeData;

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DateTimeStruct> a_left,
		std::optional<DateTimeStruct> a_right)
	{
		return REX::GetNumericStrongOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DateTimeStruct> a_left,
		std::optional<DateTimeStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
