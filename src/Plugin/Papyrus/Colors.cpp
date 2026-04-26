#include "Plugin/Papyrus/Colors.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"

namespace Plugin::Papyrus::Colors
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;

	static ColorStruct FromHex(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return ColorStruct::FromHex(a_value);
	}

	static std::uint32_t ToHex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return a_color->ToHex();
	}

	static std::optional<ColorStruct> FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		const auto hexNumber = REX::FromString<std::uint32_t>(a_value, REX::IntFormat::kHexadecimal);
		if (!hexNumber.has_value()) {
			return std::nullopt;
		}

		return ColorStruct::FromHex(hexNumber.value());
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		auto result = REX::ToString(a_color->ToHex(), REX::IntFormat::kHexadecimal);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromHex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToHex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
	}
}
