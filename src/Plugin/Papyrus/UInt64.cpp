#include "Plugin/Papyrus/UInt64.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::UInt64
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static std::optional<LongStruct> Copy(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		return a_value;
	}

	static bool ToBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		return LongStruct::ToBool(a_value.value());
	}

	static std::int32_t ToInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		return LongStruct::ToInt32(a_value.value());
	}

	static REX::Float32 ToFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0.0_f32;
		}

		return LongStruct::ToFloat32(a_value.value());
	}

	static std::optional<DoubleStruct> ToDouble(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		return DoubleStruct::FromFloat64(LongStruct::ToFloat64(a_value.value()));
	}

	static std::optional<LongStruct> FromString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		REX::IntFormat a_base,
		std::optional<LongStruct> a_default)
	{
		const auto result = REX::FromString<std::uint64_t>(a_value, a_base);
		if (!result.has_value()) {
			return a_default;
		}

		return LongStruct::FromUInt64(result.value());
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		REX::IntFormat a_base)
	{
		if (!a_value.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		auto result = REX::ToString(LongStruct::ToUInt64(a_value.value()), a_base);
		if (!result.has_value()) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		return std::move(result).value();
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::string_view a_format,
		std::string_view a_default)
	{
		if (!a_value.has_value()) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());

		auto result = REX::SafeVFormat(a_format, REX::CreateFormatArgs(value64));
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().what(), a_stackId);
			return std::string(a_default);
		}

		return std::move(result).value();
	}

	static std::optional<LongStruct> MinValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return LongStruct::FromUInt64(std::numeric_limits<std::uint64_t>::min());
	}

	static std::optional<LongStruct> MaxValue(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return LongStruct::FromUInt64(std::numeric_limits<std::uint64_t>::max());
	}

	static std::optional<LongStruct> Add(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(left64 + right64);
	}

	static std::optional<LongStruct> Subtract(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(left64 - right64);
	}

	static std::optional<LongStruct> Multiply(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(left64 * right64);
	}

	static std::optional<LongStruct> Divide(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		if (right64 == 0) {
			return std::nullopt;
		}

		return LongStruct::FromUInt64(left64 / right64);
	}

	static std::optional<LongStruct> Modulo(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());
		const auto result64 = left64 % right64;

		return LongStruct::FromUInt64(result64 + right64);
	}

	static std::optional<LongStruct> LeftShift(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::int32_t a_shift)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		return LongStruct::FromUInt64(value64 << a_shift);
	}

	static std::optional<LongStruct> RightShift(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::int32_t a_shift)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		return LongStruct::FromUInt64(value64 >> a_shift);
	}

	static std::optional<LongStruct> BitwiseAnd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());
		const auto result64 = left64 & right64;

		return LongStruct::FromUInt64(result64);
	}

	static std::optional<LongStruct> BitwiseOr(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());
		const auto result64 = left64 | right64;

		return LongStruct::FromUInt64(result64);
	}

	static std::optional<LongStruct> BitwiseXor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());
		const auto result64 = left64 ^ right64;

		return LongStruct::FromUInt64(result64);
	}

	static std::optional<LongStruct> BitwiseNot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		const auto result64 = ~value64;

		return LongStruct::FromUInt64(result64);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::optional<LongStruct> a_flag)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return false;
		}

		if (!a_flag.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return false;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		const auto flag64 = LongStruct::ToUInt64(a_flag.value());
		return (value64 & flag64) != 0;
	}

	static std::optional<LongStruct> SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::optional<LongStruct> a_flag,
		bool a_set)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_flag.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		const auto flag64 = LongStruct::ToUInt64(a_flag.value());
		const auto result64 = a_set ? (value64 | flag64) : (value64 & ~flag64);

		return LongStruct::FromUInt64(result64);
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		return REX::GetNumericStrongOrdering(a_left <=> a_right);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		return REX::Equals(a_left, a_right);
	}

	static std::optional<LongStruct> Min(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(std::min(left64, right64));
	}

	static std::optional<LongStruct> Max(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(std::max(left64, right64));
	}

	static std::optional<LongStruct> Clamp(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::optional<LongStruct> a_min,
		std::optional<LongStruct> a_max)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_min.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_max.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_03_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		const auto min64 = LongStruct::ToUInt64(a_min.value());
		const auto max64 = LongStruct::ToUInt64(a_max.value());

		if (min64 > max64) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		return LongStruct::FromUInt64(std::clamp(value64, min64, max64));
	}

	static std::optional<LongStruct> Sign(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		return LongStruct::FromUInt64(std::signbit(value64) ? 0 : 1);
	}

	static std::optional<LongStruct> CopySign(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value,
		std::optional<LongStruct> a_sign)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_sign.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		const auto sign = LongStruct::ToUInt64(a_sign.value());

		return LongStruct::FromUInt64(static_cast<std::uint64_t>(std::copysign(value64, sign)));
	}

	static std::optional<LongStruct> Lcm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(std::lcm(left64, right64));
	}

	static std::optional<LongStruct> Gcd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_left,
		std::optional<LongStruct> a_right)
	{
		if (!a_left.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_right.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto left64 = LongStruct::ToUInt64(a_left.value());
		const auto right64 = LongStruct::ToUInt64(a_right.value());

		return LongStruct::FromUInt64(std::gcd(left64, right64));
	}

	static std::optional<LongStruct> Fib(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		return LongStruct::FromUInt64(REX::Fib(value64));
	}

	static std::optional<LongStruct> Fact(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto value64 = LongStruct::ToUInt64(a_value.value());
		return LongStruct::FromUInt64(REX::Fact(value64));
	}

	static std::optional<LongStruct> Perm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_n,
		std::optional<LongStruct> a_r)
	{
		if (!a_n.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_r.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto n64 = LongStruct::ToUInt64(a_n.value());
		const auto r64 = LongStruct::ToUInt64(a_r.value());

		return LongStruct::FromUInt64(REX::Perm(n64, r64));
	}

	static std::optional<LongStruct> Comb(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_n,
		std::optional<LongStruct> a_r)
	{
		if (!a_n.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_r.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto n64 = LongStruct::ToUInt64(a_n.value());
		const auto r64 = LongStruct::ToUInt64(a_r.value());

		return LongStruct::FromUInt64(REX::Comb(n64, r64));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToInt);
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
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Lcm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Gcd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Fib);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Fact);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Perm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Comb);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CopySign);
	}
}
