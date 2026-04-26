#include "Plugin/Papyrus/Float32.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::Float32
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static REX::Float32 Copy(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return a_value;
	}

	static bool ToBool(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return static_cast<bool>(a_value);
	}

	static std::int32_t ToInt(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return static_cast<std::int32_t>(a_value);
	}

	static LongStruct ToLong(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return LongStruct::FromFloat32(a_value);
	}

	static DoubleStruct ToDouble(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return DoubleStruct::FromFloat32(a_value);
	}

	static REX::Float32 FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		REX::Float32 a_default)
	{
		const auto result = REX::FromString<REX::Float32>(a_value);
		if (!result.has_value()) {
			return a_default;
		}

		return result.value();
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value,
		std::int32_t a_precision)
	{
		const auto result = REX::ToString(a_value, REX::FloatFormat::kDefault, a_precision);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return result.value();
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value,
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

	static std::int32_t MinDecimals(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::digits10;
	}

	static std::int32_t MaxDecimals(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::max_digits10;
	}

	static REX::Float32 MinValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::min();
	}

	static REX::Float32 MaxValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::max();
	}

	static REX::Float32 Pi(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numbers::pi_v<REX::Float32>;
	}

	static REX::Float32 E(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numbers::e_v<REX::Float32>;
	}

	static REX::Float32 Tau(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return REX::TAU<REX::Float32>;
	}

	static REX::Float32 Epsilon(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::denorm_min();
	}

	static REX::Float32 NaN(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::quiet_NaN();
	}

	static REX::Float32 PositiveInfinity(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float32>::infinity();
	}

	static REX::Float32 NegativeInfinity(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return -std::numeric_limits<REX::Float32>::infinity();
	}

	static bool IsNaN(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return std::isnan(a_value);
	}

	static bool IsInfinity(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return std::isinf(a_value);
	}

	static bool IsPositiveInfinity(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return std::isinf(a_value) && a_value > 0.0_f32;
	}

	static bool IsNegativeInfinity(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		return std::isinf(a_value) && a_value < 0.0_f32;
	}

	static REX::Float64 Add(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return a_left + a_right;
	}

	static REX::Float64 Subtract(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return a_left - a_right;
	}

	static REX::Float64 Multiply(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return a_left * a_right;
	}

	static REX::Float64 Divide(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		if (a_right == 0.0_f64) {
			return 0.0_f64;
		}

		return a_left / a_right;
	}

	static REX::Float64 Modulo(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return REX::Mod(a_left, a_right);
	}

	static REX::Float64 Negate(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return -a_value;
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return REX::GetNumericPartialOrdering(a_left <=> a_right);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	static REX::Float64 Abs(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::abs(a_value);
	}

	static REX::Float64 Min(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return std::min(a_left, a_right);
	}

	static REX::Float64 Max(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_left,
		REX::Float64 a_right)
	{
		return std::max(a_left, a_right);
	}

	static REX::Float64 Clamp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value,
		REX::Float64 a_min,
		REX::Float64 a_max)
	{
		if (a_min > a_max) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0.0_f64;
		}

		return std::clamp(a_value, a_min, a_max);
	}

	static REX::Float64 Sign(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::signbit(a_value) ? -1.0_f64 : 1.0_f64;
	}

	static REX::Float64 CopySign(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value,
		REX::Float64 a_sign)
	{
		return std::copysign(a_value, a_sign);
	}

	static REX::Float64 Lerp(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_start,
		REX::Float64 a_end,
		REX::Float64 a_factor)
	{
		return std::lerp(a_start, a_end, a_factor);
	}

	static REX::Float64 Ceil(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::ceil(a_value);
	}

	static REX::Float64 Floor(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::floor(a_value);
	}

	static REX::Float64 Round(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value,
		std::int32_t a_decimals)
	{
		constexpr auto MIN_DECIMALS = 0;
		constexpr auto MAX_DECIMALS = std::numeric_limits<REX::Float32>::digits10;

		if (a_decimals < MIN_DECIMALS || a_decimals > MAX_DECIMALS) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DECIMALS_OUT_OF_RANGE, a_stackId, a_decimals, MIN_DECIMALS, MAX_DECIMALS);
			return 0.0_f64;
		}

		return REX::Round(a_value, a_decimals);
	}

	static REX::Float64 Trunc(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::trunc(a_value);
	}

	static REX::Float64 Pow(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_base,
		REX::Float64 a_exponent)
	{
		return std::pow(a_base, a_exponent);
	}

	static REX::Float64 Sqrt(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::sqrt(a_value);
	}

	static REX::Float64 Exp(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::exp(a_value);
	}

	static REX::Float64 Exp2(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::exp2(a_value);
	}

	static REX::Float64 Expm1(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::expm1(a_value);
	}

	static REX::Float64 Log(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value,
		REX::Float64 a_base)
	{
		return std::log(a_value) / std::log(a_base);
	}

	static REX::Float64 Ln(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::log(a_value);
	}

	static REX::Float64 Log2(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::log2(a_value);
	}

	static REX::Float64 Log10(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::log10(a_value);
	}

	static REX::Float64 Log1p(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::log1p(a_value);
	}

	static REX::Float64 Erf(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::erf(a_value);
	}

	static REX::Float64 Erfc(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::erfc(a_value);
	}

	static REX::Float64 Gamma(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::tgamma(a_value);
	}

	static REX::Float64 Lgamma(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return REX::Lgamma(a_value);
	}

	static REX::Float64 DegToRad(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return REX::DegToRad(a_value);
	}

	static REX::Float64 RadToDeg(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return REX::RadToDeg(a_value);
	}

	static REX::Float64 Cbrt(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::cbrt(a_value);
	}

	static REX::Float64 Hypot(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_x,
		REX::Float64 a_y)
	{
		return std::hypot(a_x, a_y);
	}

	static REX::Float64 Sin(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::sin(a_value);
	}

	static REX::Float64 Cos(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::cos(a_value);
	}

	static REX::Float64 Tan(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::tan(a_value);
	}

	static REX::Float64 Sinh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::sinh(a_value);
	}

	static REX::Float64 Cosh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::cosh(a_value);
	}

	static REX::Float64 Tanh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::tanh(a_value);
	}

	static REX::Float64 Asin(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::asin(a_value);
	}

	static REX::Float64 Acos(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::acos(a_value);
	}

	static REX::Float64 Atan(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::atan(a_value);
	}

	static REX::Float64 Atan2(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_y,
		REX::Float64 a_x)
	{
		return std::atan2(a_y, a_x);
	}

	static REX::Float64 Asinh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::asinh(a_value);
	}

	static REX::Float64 Acosh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::acosh(a_value);
	}

	static REX::Float64 Atanh(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_value)
	{
		return std::atanh(a_value);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToLong);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToDouble);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Format);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MinDecimals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MaxDecimals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MinValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MaxValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Pi);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, E);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Tau);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Epsilon);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, NaN);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PositiveInfinity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, NegativeInfinity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNaN);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsInfinity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPositiveInfinity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNegativeInfinity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Add);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Subtract);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Multiply);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Divide);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Modulo);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Negate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Abs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Min);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Max);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Clamp);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sign);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CopySign);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Lerp);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Ceil);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Floor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Round);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Trunc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Pow);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sqrt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exp);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exp2);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Expm1);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Log);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Ln);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Log2);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Log10);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Log1p);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Erf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Erfc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Gamma);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Lgamma);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DegToRad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RadToDeg);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cbrt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Hypot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cos);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Tan);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sinh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cosh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Tanh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Asin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Acos);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Atan);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Atan2);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Asinh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Acosh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Atanh);
	}
}
