#include "Plugin/Papyrus/Versions.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Versions.hpp"

namespace Plugin::Papyrus::Versions
{
	using VersionStruct = ::Plugin::Internal::Structs::Versions::VersionData;

	static std::uint32_t Pack(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<VersionStruct> a_value)
	{
		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return a_value->Pack<std::uint32_t>();
	}

	static VersionStruct Unpack(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return VersionStruct::Unpack<std::uint32_t>(a_value);
	}

	static std::optional<VersionStruct> FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		const auto version = VersionStruct::FromString(a_value);
		if (!version.has_value()) {
			return std::nullopt;
		}

		return version.value();
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<VersionStruct> a_value)
	{
		if (!a_value.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		return a_value->ToString<char>();
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<VersionStruct> a_left,
		std::optional<VersionStruct> a_right)
	{
		return REX::GetNumericStrongOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<VersionStruct> a_left,
		std::optional<VersionStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Pack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Unpack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
