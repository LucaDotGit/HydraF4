#include "Plugin/Papyrus/Boolean.hpp"

#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::Boolean
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static std::int32_t ToInt(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		return static_cast<std::int32_t>(a_value);
	}

	static LongStruct ToLong(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		return LongStruct::FromBool(a_value);
	}

	static REX::Float32 ToFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		return static_cast<REX::Float32>(a_value);
	}

	static DoubleStruct ToDouble(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		return DoubleStruct::FromBool(a_value);
	}

	static bool FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		bool a_default)
	{
		const auto result = REX::FromString<bool>(a_value);
		if (!result.has_value()) {
			return a_default;
		}

		return result.value();
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		constexpr auto BOOL_FORMAT = REX::EnumSet(REX::BoolFormat::kTrueFalse, REX::BoolFormat::kCapitalize);

		auto result = REX::ToString(a_value, BOOL_FORMAT.get());
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value,
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

	static RE::BSFixedString FalseString(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto FALSE_STRING = RE::BSFixedString(REX::CAPITAL_FALSE_STRING);
		return FALSE_STRING;
	}

	static RE::BSFixedString TrueString(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto TRUE_STRING = RE::BSFixedString(REX::CAPITAL_TRUE_STRING);
		return TRUE_STRING;
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_left,
		bool a_right)
	{
		return REX::GetNumericStrongOrdering(REX::Compare(a_left, a_right));
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_left,
		bool a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToLong);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToDouble);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Format);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FalseString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TrueString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
	}
}
