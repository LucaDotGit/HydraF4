#include "Plugin/Papyrus/Hashing.hpp"

#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::Hashing
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;

	static std::uint32_t HashBytesCrc32(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		return REX::HashCrc32(std::span<const std::byte>(a_input));
	}

	static LongStruct HashBytesCrc64(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		return LongStruct::FromUInt64(REX::HashCrc64(std::span<const std::byte>(a_input)));
	}

	static std::uint32_t HashBytesFnv1a32(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		return REX::HashFnv1a32(std::span<const std::byte>(a_input));
	}

	static LongStruct HashBytesFnv1a64(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		return LongStruct::FromUInt64(REX::HashFnv1a64(std::span<const std::byte>(a_input)));
	}

	static std::uint32_t HashStringCrc32(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);
		return REX::HashCrc32(caseInsensitiveInput);
	}

	static LongStruct HashStringCrc64(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);
		return LongStruct::FromUInt64(REX::HashCrc64(caseInsensitiveInput));
	}

	static std::uint32_t HashStringFnv1a32(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);
		return REX::HashFnv1a32(caseInsensitiveInput);
	}

	static LongStruct HashStringFnv1a64(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);
		return LongStruct::FromUInt64(REX::HashFnv1a64(caseInsensitiveInput));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesCrc32);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesCrc64);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesFnv1a32);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesFnv1a64);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringCrc32);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringCrc64);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringFnv1a32);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringFnv1a64);
	}
}
