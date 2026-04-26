#include "Plugin/Papyrus/Char.hpp"

namespace Plugin::Papyrus::Char
{
	static char FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		char a_default)
	{
		if (a_value.size() != 1) {
			return a_default;
		}

		return a_value.front();
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		char a_value)
	{
		return { 1, a_value };
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		char a_value,
		std::string_view a_format,
		std::string_view a_default)
	{
		auto result = REX::SafeVFormat(a_format, REX::CreateFormatArgs(a_value));
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().what(), a_stackId);
			return std::string(a_default);
		}

		return std::move(result).value();
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		char a_left,
		char a_right)
	{
		return REX::GetNumericStrongOrdering(REX::Compare(a_left, a_right));
	}

	static std::int32_t CompareIgnoreCase(RE::BSScript::StaticTag /*a_staticTag*/,
		char a_left,
		char a_right)
	{
		return REX::GetNumericStrongOrdering(REX::CompareIgnoreCase(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		char a_left,
		char a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	static bool EqualsIgnoreCase(RE::BSScript::StaticTag /*a_staticTag*/,
		char a_left,
		char a_right)
	{
		return REX::EqualsIgnoreCase(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Format);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CompareIgnoreCase);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, EqualsIgnoreCase);
	}
}
