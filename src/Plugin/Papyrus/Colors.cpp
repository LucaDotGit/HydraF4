#include "Plugin/Papyrus/Colors.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"

namespace Plugin::Papyrus::Colors
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;

	static ColorStruct FromHexRgb(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return ColorStruct::FromHexRgb(a_value);
	}

	static std::uint32_t ToHexRgb(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return a_color->ToHexRgb();
	}

	static ColorStruct FromHexRgba(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return ColorStruct::FromHexRgba(a_value);
	}

	static std::uint32_t ToHexRgba(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return a_color->ToHexRgba();
	}

	static std::optional<ColorStruct> FromRgbString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		const auto hexNumber = REX::FromString<std::uint32_t>(a_value, REX::IntFormat::kHexadecimal);
		if (!hexNumber.has_value()) {
			return std::nullopt;
		}

		return ColorStruct::FromHexRgb(hexNumber.value());
	}

	static std::string ToRgbString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		auto result = REX::ToString(a_color->ToHexRgb(), REX::IntFormat::kHexadecimal);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	static std::optional<ColorStruct> FromRgbaString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		const auto hexNumber = REX::FromString<std::uint32_t>(a_value, REX::IntFormat::kHexadecimal);
		if (!hexNumber.has_value()) {
			return std::nullopt;
		}

		return ColorStruct::FromHexRgba(hexNumber.value());
	}

	static std::string ToRgbaString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		auto result = REX::ToString(a_color->ToHexRgba(), REX::IntFormat::kHexadecimal);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromHexRgb);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToHexRgb);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromHexRgba);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToHexRgba);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromRgbString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToRgbString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromRgbaString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToRgbaString);
	}
}
