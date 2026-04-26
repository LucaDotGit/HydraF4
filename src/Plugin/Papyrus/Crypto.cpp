#include "Plugin/Papyrus/Crypto.hpp"

namespace Plugin::Papyrus::Crypto
{
	static std::optional<std::vector<std::byte>> HashBytesMd5(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		auto result = REX::HashMd5(a_input);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<std::byte>> HashBytesSha1(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		auto result = REX::HashSha1(a_input);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<std::byte>> HashBytesSha256(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		auto result = REX::HashSha256(a_input);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<std::byte>> HashBytesSha384(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		auto result = REX::HashSha384(a_input);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<std::byte>> HashBytesSha512(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::byte> a_input)
	{
		auto result = REX::HashSha512(a_input);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::string HashStringMd5(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);

		auto result = REX::HashMd5(caseInsensitiveInput);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	static std::string HashStringSha1(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);

		auto result = REX::HashSha1(caseInsensitiveInput);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	static std::string HashStringSha256(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);

		auto result = REX::HashSha256(caseInsensitiveInput);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	static std::string HashStringSha384(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);

		auto result = REX::HashSha384(caseInsensitiveInput);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	static std::string HashStringSha512(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string a_input)
	{
		const auto caseInsensitiveInput = REX::ToLower(a_input);

		auto result = REX::HashSha512(caseInsensitiveInput);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesMd5);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesSha1);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesSha256);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesSha384);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashBytesSha512);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringMd5);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringSha1);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringSha256);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringSha384);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HashStringSha512);
	}
}
