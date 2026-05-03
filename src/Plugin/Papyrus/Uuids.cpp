#include "Plugin/Papyrus/Uuids.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Uuids.hpp"

namespace Plugin::Papyrus::Uuids
{
	using UuidStruct = ::Plugin::Internal::Structs::Uuids::UuidData;

	static UuidStruct Generate(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return UuidStruct::Generate();
	}

	static std::optional<UuidStruct> FromBytes(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::uint8_t> a_values)
	{
		return UuidStruct::FromBytes(a_values);
	}

	static std::optional<std::vector<std::uint8_t>> ToBytes(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<UuidStruct> a_uuid)
	{
		if (!a_uuid.has_value()) {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		return std::vector<std::uint8_t>{ a_uuid->begin(), a_uuid->end() };
	}

	static std::optional<UuidStruct> FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return UuidStruct::FromString(a_value);
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<UuidStruct> a_uuid)
	{
		if (!a_uuid.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		return a_uuid->ToString();
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<UuidStruct> a_left,
		std::optional<UuidStruct> a_right)
	{
		return REX::GetNumericStrongOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<UuidStruct> a_left,
		std::optional<UuidStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Generate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromBytes);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToBytes);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
