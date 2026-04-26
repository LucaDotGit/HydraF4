#include "Plugin/Papyrus/Vectors4.hpp"

#include "Plugin/Internal/Structs/Vectors4.hpp"

namespace Plugin::Papyrus::Vectors4
{
	using Vector4Struct = ::Plugin::Internal::Structs::Vectors4::Vector4Data;

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<Vector4Struct> a_left,
		std::optional<Vector4Struct> a_right)
	{
		return REX::GetNumericPartialOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<Vector4Struct> a_left,
		std::optional<Vector4Struct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
