#include "Plugin/Papyrus/Vectors3.hpp"

#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Vectors3
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<Vector3Struct> a_left,
		std::optional<Vector3Struct> a_right)
	{
		return REX::GetNumericPartialOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<Vector3Struct> a_left,
		std::optional<Vector3Struct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
