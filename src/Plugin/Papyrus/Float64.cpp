#include "Plugin/Papyrus/Float64.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::Float64
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static DoubleStruct Copy(RE::BSScript::StaticTag /*a_staticTag*/,
		DoubleStruct a_value)
	{
		return a_value;
	}

	static bool ToBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		return DoubleStruct::ToBool(a_value.value());
	}

	static std::int32_t ToInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return DoubleStruct::ToInt32(a_value.value());
	}

	static REX::Float32 ToFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0.0_f32;
		}

		return DoubleStruct::ToFloat32(a_value.value());
	}

	static std::optional<LongStruct> ToLong(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		return LongStruct::FromInt64(DoubleStruct::ToInt64(a_value.value()));
	}

	static std::optional<DoubleStruct> FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::optional<DoubleStruct> a_default)
	{
		const auto result = REX::FromString<REX::Float64>(a_value);
		if (!result.has_value()) {
			return a_default;
		}

		return DoubleStruct::FromFloat64(result.value());
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::int32_t a_precision)
	{
		if (!a_value.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		auto result = REX::ToString(DoubleStruct::ToFloat64(a_value.value()), REX::FloatFormat::kDefault, a_precision);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::string_view a_format,
		std::string_view a_default)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::string(RE::BSScript::NONE_STRING);
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());

		auto result = REX::SafeVFormat(a_format, REX::CreateFormatArgs(value64));
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().what(), a_stackId);
			return std::string(a_default);
		}

		return std::move(result).value();
	}

	static std::int32_t MinDecimals(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float64>::digits10;
	}

	static std::int32_t MaxDecimals(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::numeric_limits<REX::Float64>::max_digits10;
	}

	static std::optional<DoubleStruct> MinValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numeric_limits<REX::Float64>::min());
	}

	static std::optional<DoubleStruct> MaxValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numeric_limits<REX::Float64>::max());
	}

	static std::optional<DoubleStruct> Pi(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numbers::pi_v<REX::Float64>);
	}

	static std::optional<DoubleStruct> E(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numbers::e_v<REX::Float64>);
	}

	static std::optional<DoubleStruct> Tau(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(REX::TAU<REX::Float64>);
	}

	static std::optional<DoubleStruct> Epsilon(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numeric_limits<REX::Float64>::denorm_min());
	}

	static std::optional<DoubleStruct> NaN(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numeric_limits<REX::Float64>::quiet_NaN());
	}

	static std::optional<DoubleStruct> PositiveInfinity(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(std::numeric_limits<REX::Float64>::infinity());
	}

	static std::optional<DoubleStruct> NegativeInfinity(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return DoubleStruct::FromFloat64(-std::numeric_limits<REX::Float64>::infinity());
	}

	static bool IsNaN(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		return std::isnan(DoubleStruct::ToFloat64(a_value.value()));
	}

	static bool IsInfinity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		return std::isinf(DoubleStruct::ToFloat64(a_value.value()));
	}

	static bool IsPositiveInfinity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return std::isinf(value64) && value64 > 0.0_f64;
	}

	static bool IsNegativeInfinity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return std::isinf(value64) && value64 < 0.0_f64;
	}

	static std::optional<DoubleStruct> Add(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(left64 + right64);
	}

	static std::optional<DoubleStruct> Subtract(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(left64 - right64);
	}

	static std::optional<DoubleStruct> Multiply(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(left64 * right64);
	}

	static std::optional<DoubleStruct> Divide(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		if (right64 == 0.0_f64) {
			return DoubleStruct::FromFloat64(0.0_f64);
		}

		return DoubleStruct::FromFloat64(left64 / right64);
	}

	static std::optional<DoubleStruct> Modulo(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(REX::Mod(left64, right64));
	}

	static std::optional<DoubleStruct> Negate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		return DoubleStruct::FromFloat64(-DoubleStruct::ToFloat64(a_value.value()));
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		return REX::GetNumericPartialOrdering(a_left <=> a_right);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	static std::optional<DoubleStruct> Abs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::abs(value64));
	}

	static std::optional<DoubleStruct> Min(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(std::min(left64, right64));
	}

	static std::optional<DoubleStruct> Max(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_left,
		std::optional<DoubleStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = DoubleStruct::ToFloat64(a_left.value());
		const auto right64 = DoubleStruct::ToFloat64(a_right.value());

		return DoubleStruct::FromFloat64(std::max(left64, right64));
	}

	static std::optional<DoubleStruct> Clamp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::optional<DoubleStruct> a_min,
		std::optional<DoubleStruct> a_max)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_min.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_max.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		const auto min64 = DoubleStruct::ToFloat64(a_min.value());
		const auto max64 = DoubleStruct::ToFloat64(a_max.value());

		if (min64 > max64) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		return DoubleStruct::FromFloat64(std::clamp(value64, min64, max64));
	}

	static std::optional<DoubleStruct> Sign(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::signbit(value64) ? -1.0_f64 : 1.0_f64);
	}

	static std::optional<DoubleStruct> CopySign(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::optional<DoubleStruct> a_sign)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_sign.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		const auto sign = DoubleStruct::ToFloat64(a_sign.value());

		return DoubleStruct::FromFloat64(std::copysign(value64, sign));
	}

	static std::optional<DoubleStruct> Lerp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_start,
		std::optional<DoubleStruct> a_end,
		std::optional<DoubleStruct> a_alpha)
	{
		if (!a_start.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_end.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_alpha.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_03_NULL, a_stackId);
			return std::nullopt;
		}

		const auto start = DoubleStruct::ToFloat64(a_start.value());
		const auto end = DoubleStruct::ToFloat64(a_end.value());
		const auto alpha = DoubleStruct::ToFloat64(a_alpha.value());

		return DoubleStruct::FromFloat64(std::lerp(start, end, alpha));
	}

	static std::optional<DoubleStruct> Ceil(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::ceil(value64));
	}

	static std::optional<DoubleStruct> Floor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::floor(value64));
	}

	static std::optional<DoubleStruct> Round(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::int32_t a_decimals)
	{
		constexpr auto MIN_DECIMALS = 0;
		constexpr auto MAX_DECIMALS = std::numeric_limits<REX::Float64>::digits10;

		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		if (a_decimals < MIN_DECIMALS || a_decimals > MAX_DECIMALS) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DECIMALS_OUT_OF_RANGE, a_stackId, a_decimals, MIN_DECIMALS, MAX_DECIMALS);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(REX::Round(value64, a_decimals));
	}

	static std::optional<DoubleStruct> Trunc(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::trunc(value64));
	}

	static std::optional<DoubleStruct> Pow(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_base,
		std::optional<DoubleStruct> a_exponent)
	{
		if (!a_base.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_exponent.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto base = DoubleStruct::ToFloat64(a_base.value());
		const auto exponent = DoubleStruct::ToFloat64(a_exponent.value());

		return DoubleStruct::FromFloat64(std::pow(base, exponent));
	}

	static std::optional<DoubleStruct> Sqrt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::sqrt(value64));
	}

	static std::optional<DoubleStruct> Exp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::exp(value64));
	}

	static std::optional<DoubleStruct> Exp2(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::exp2(value64));
	}

	static std::optional<DoubleStruct> Expm1(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::expm1(value64));
	}

	static std::optional<DoubleStruct> Log(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value,
		std::optional<DoubleStruct> a_base)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_base.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		const auto base = DoubleStruct::ToFloat64(a_base.value());

		return DoubleStruct::FromFloat64(std::log(value64) / std::log(base));
	}

	static std::optional<DoubleStruct> Ln(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::log(value64));
	}

	static std::optional<DoubleStruct> Log2(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::log2(value64));
	}

	static std::optional<DoubleStruct> Log10(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::log10(value64));
	}

	static std::optional<DoubleStruct> Log1p(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::log1p(value64));
	}

	static std::optional<DoubleStruct> Erf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::erf(value64));
	}

	static std::optional<DoubleStruct> Erfc(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::erfc(value64));
	}

	static std::optional<DoubleStruct> Gamma(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::tgamma(value64));
	}

	static std::optional<DoubleStruct> Lgamma(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(REX::Lgamma(value64));
	}

	static std::optional<DoubleStruct> DegToRad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(REX::DegToRad(value64));
	}

	static std::optional<DoubleStruct> RadToDeg(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(REX::RadToDeg(value64));
	}

	static std::optional<DoubleStruct> Cbrt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::cbrt(value64));
	}

	static std::optional<DoubleStruct> Hypot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_x,
		std::optional<DoubleStruct> a_y)
	{
		if (!a_x.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_y.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto x64 = DoubleStruct::ToFloat64(a_x.value());
		const auto y64 = DoubleStruct::ToFloat64(a_y.value());

		return DoubleStruct::FromFloat64(std::hypot(x64, y64));
	}

	static std::optional<DoubleStruct> Sin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::sin(value64));
	}

	static std::optional<DoubleStruct> Cos(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::cos(value64));
	}

	static std::optional<DoubleStruct> Tan(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::tan(value64));
	}

	static std::optional<DoubleStruct> Sinh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::sinh(value64));
	}

	static std::optional<DoubleStruct> Cosh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::cosh(value64));
	}

	static std::optional<DoubleStruct> Tanh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::tanh(value64));
	}

	static std::optional<DoubleStruct> Asin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::asin(value64));
	}

	static std::optional<DoubleStruct> Acos(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::acos(value64));
	}

	static std::optional<DoubleStruct> Atan(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::atan(value64));
	}

	static std::optional<DoubleStruct> Atan2(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_y,
		std::optional<DoubleStruct> a_x)
	{
		if (!a_y.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_x.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto y64 = DoubleStruct::ToFloat64(a_y.value());
		const auto x64 = DoubleStruct::ToFloat64(a_x.value());

		return DoubleStruct::FromFloat64(std::atan2(y64, x64));
	}

	static std::optional<DoubleStruct> Asinh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::asinh(value64));
	}

	static std::optional<DoubleStruct> Acosh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::acosh(value64));
	}

	static std::optional<DoubleStruct> Atanh(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = DoubleStruct::ToFloat64(a_value.value());
		return DoubleStruct::FromFloat64(std::atanh(value64));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToLong);
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
