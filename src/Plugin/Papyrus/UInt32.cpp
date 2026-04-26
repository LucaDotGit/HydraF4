#include "Plugin/Papyrus/UInt32.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::UInt32
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static std::uint32_t Copy(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return a_value;
	}

	static bool ToBool(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return static_cast<bool>(a_value);
	}

	static LongStruct ToLong(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return LongStruct::FromUInt32(a_value);
	}

	static REX::Float32 ToFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return static_cast<REX::Float32>(a_value);
	}

	static DoubleStruct ToDouble(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return DoubleStruct::FromUInt32(a_value);
	}

	static std::uint32_t FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		REX::IntFormat a_base,
		std::uint32_t a_default)
	{
		const auto result = REX::FromString<std::uint32_t>(a_value, a_base);
		if (!result.has_value()) {
			return a_default;
		}

		return result.value();
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		REX::IntFormat a_base)
	{
		auto result = REX::ToString(a_value, a_base);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
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

	static std::uint32_t MinValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<std::uint32_t>::min();
	}

	static std::uint32_t MaxValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<std::uint32_t>::max();
	}

	static std::uint32_t Add(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left + a_right;
	}

	static std::uint32_t Subtract(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left - a_right;
	}

	static std::uint32_t Multiply(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left * a_right;
	}

	static std::uint32_t Divide(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		if (a_right == 0) {
			return 0;
		}

		return a_left / a_right;
	}

	static std::uint32_t Modulo(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return REX::Mod(a_left, a_right);
	}

	static std::uint32_t LeftShift(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::int32_t a_shift)
	{
		return a_value << a_shift;
	}

	static std::uint32_t RightShift(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::int32_t a_shift)
	{
		return a_value >> a_shift;
	}

	static std::uint32_t BitwiseAnd(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left & a_right;
	}

	static std::uint32_t BitwiseOr(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left | a_right;
	}

	static std::uint32_t BitwiseXor(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return a_left ^ a_right;
	}

	static std::uint32_t BitwiseNot(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return ~a_value;
	}

	static bool HasFlag(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::uint32_t a_flag)
	{
		return (a_value & a_flag) != 0;
	}

	static std::uint32_t SetFlag(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::uint32_t a_flag,
		bool a_set)
	{
		return a_set ? (a_value | a_flag) : (a_value & ~a_flag);
	}

	static std::uint32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return REX::GetNumericStrongOrdering(a_left <=> a_right);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	static std::uint32_t Min(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return std::min(a_left, a_right);
	}

	static std::uint32_t Max(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return std::max(a_left, a_right);
	}

	static std::uint32_t Clamp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::uint32_t a_min,
		std::uint32_t a_max)
	{
		if (a_min > a_max) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0;
		}

		return std::clamp(a_value, a_min, a_max);
	}

	static std::int32_t Sign(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return std::signbit(a_value) ? -1 : 1;
	}

	static std::uint32_t CopySign(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value,
		std::uint32_t a_sign)
	{
		return static_cast<std::uint32_t>(std::copysign(a_value, a_sign));
	}

	static std::uint32_t Lcm(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return std::lcm(a_left, a_right);
	}

	static std::uint32_t Gcd(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_left,
		std::uint32_t a_right)
	{
		return std::gcd(a_left, a_right);
	}

	static std::uint32_t Fib(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return REX::Fib(a_value);
	}

	static std::uint32_t Fact(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_value)
	{
		return REX::Fact(a_value);
	}

	static std::uint32_t Perm(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_n,
		std::uint32_t a_r)
	{
		return REX::Perm(a_n, a_r);
	}

	static std::uint32_t Comb(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_n,
		std::uint32_t a_r)
	{
		return REX::Comb(a_n, a_r);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToLong);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToDouble);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Format);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MinValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MaxValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Add);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Subtract);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Multiply);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Divide);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Modulo);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, LeftShift);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RightShift);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, BitwiseAnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, BitwiseOr);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, BitwiseXor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, BitwiseNot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Min);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Max);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Clamp);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sign);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CopySign);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Lcm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Gcd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Fib);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Fact);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Perm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Comb);
	}
}
