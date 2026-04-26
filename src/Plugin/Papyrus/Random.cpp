#include "Plugin/Papyrus/Random.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Float64.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::Random
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;
	using DoubleStruct = ::Plugin::Internal::Structs::Float64::DoubleData;

	static std::uint32_t RandomSeed(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return REX::RandomSeed<std::uint32_t>();
	}

	static bool RandomBool(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto random = REX::Random64();
		return random.NextBool();
	}

	static bool SeededBool(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_seed)
	{
		auto random = REX::Random64(a_seed);
		return random.NextBool();
	}

	static char RandomAlphanumericChar(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto random = REX::Random64();
		return random.NextAlphanumericChar<char>();
	}

	static char SeededAlphanumericChar(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_seed)
	{
		auto random = REX::Random64(a_seed);
		return random.NextAlphanumericChar<char>();
	}

	static std::int32_t RandomInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_minInclusive,
		std::int32_t a_maxInclusive)
	{
		if (a_minInclusive > a_maxInclusive) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0;
		}

		auto random = REX::Random64();
		return random.NextInt(a_minInclusive, a_maxInclusive);
	}

	static std::int32_t SeededInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_minInclusive,
		std::int32_t a_maxInclusive,
		std::uint32_t a_seed)
	{
		if (a_minInclusive > a_maxInclusive) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0;
		}

		auto random = REX::Random64(a_seed);
		return random.NextInt(a_minInclusive, a_maxInclusive);
	}

	static std::optional<LongStruct> RandomLong(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_minInclusive,
		std::optional<LongStruct> a_maxInclusive)
	{
		if (!a_minInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_maxInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto minValue = LongStruct::ToInt64(a_minInclusive.value());
		const auto maxValue = LongStruct::ToInt64(a_maxInclusive.value());

		if (minValue > maxValue) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		auto random = REX::Random64();
		auto result = random.NextInt(minValue, maxValue);
		return LongStruct::FromInt64(result);
	}

	static std::optional<LongStruct> SeededLong(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<LongStruct> a_minInclusive,
		std::optional<LongStruct> a_maxInclusive,
		std::uint32_t a_seed)
	{
		if (!a_minInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_maxInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto minValue = LongStruct::ToInt64(a_minInclusive.value());
		const auto maxValue = LongStruct::ToInt64(a_maxInclusive.value());

		if (minValue > maxValue) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		auto random = REX::Random64(a_seed);
		auto result = random.NextInt(minValue, maxValue);
		return LongStruct::FromInt64(result);
	}

	static REX::Float32 RandomFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_minInclusive,
		REX::Float32 a_maxExclusive)
	{
		if (a_minInclusive > a_maxExclusive) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0.0_f32;
		}

		auto random = REX::Random64();
		return random.NextFloat(a_minInclusive, a_maxExclusive);
	}

	static REX::Float32 SeededFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_minInclusive,
		REX::Float32 a_maxExclusive,
		std::uint32_t a_seed)
	{
		if (a_minInclusive > a_maxExclusive) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return 0.0_f32;
		}

		auto random = REX::Random64(a_seed);
		return random.NextFloat(a_minInclusive, a_maxExclusive);
	}

	static std::optional<DoubleStruct> RandomDouble(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_minInclusive,
		std::optional<DoubleStruct> a_maxExclusive)
	{
		if (!a_minInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_maxExclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto minValue = DoubleStruct::ToFloat64(a_minInclusive.value());
		const auto maxValue = DoubleStruct::ToFloat64(a_maxExclusive.value());

		if (minValue > maxValue) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		auto random = REX::Random64();
		auto result = random.NextFloat(minValue, maxValue);
		return DoubleStruct::FromFloat64(result);
	}

	static std::optional<DoubleStruct> SeededDouble(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<DoubleStruct> a_minInclusive,
		std::optional<DoubleStruct> a_maxExclusive,
		std::uint32_t a_seed)
	{
		if (!a_minInclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_maxExclusive.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return std::nullopt;
		}

		const auto minValue = DoubleStruct::ToFloat64(a_minInclusive.value());
		const auto maxValue = DoubleStruct::ToFloat64(a_maxExclusive.value());

		if (minValue > maxValue) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MINIMUM_GREATER_THAN_MAXIMUM, a_stackId);
			return std::nullopt;
		}

		auto random = REX::Random64(a_seed);
		auto result = random.NextFloat(minValue, maxValue);
		return DoubleStruct::FromFloat64(result);
	}

	static std::string RandomString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_size,
		std::string_view a_characters)
	{
		if (a_size < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SIZE_NEGATIVE, a_stackId);
			return {};
		}

		auto random = REX::Random64();
		return random.NextString<char>(a_size, a_characters);
	}

	static std::string SeededString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_size,
		std::string_view a_characters,
		std::uint32_t a_seed)
	{
		if (a_size < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SIZE_NEGATIVE, a_stackId);
			return {};
		}

		auto random = REX::Random64(a_seed);
		return random.NextString<char>(a_size, a_characters);
	}

	static std::string RandomAlphanumericString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_size)
	{
		if (a_size < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SIZE_NEGATIVE, a_stackId);
			return {};
		}

		auto random = REX::Random64();
		return random.NextAlphanumericString<char>(a_size);
	}

	static std::string SeededAlphanumericString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_size,
		std::uint32_t a_seed)
	{
		if (a_size < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SIZE_NEGATIVE, a_stackId);
			return {};
		}

		auto random = REX::Random64(a_seed);
		return random.NextAlphanumericString<char>(a_size);
	}

	static RE::BSScript::UniqueVariant RandomElement(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.size() == 0) {
			return nullptr;
		}

		auto random = REX::Random64();

		const auto* element = random.NextElement(a_array);
		if (!element) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(element);
	}

	static RE::BSScript::UniqueVariant SeededElement(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::uint32_t a_seed)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.size() == 0) {
			return nullptr;
		}

		auto random = REX::Random64(a_seed);

		const auto* element = random.NextElement(a_array);
		if (!element) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(element);
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RandomArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.size() < 2) {
			return a_array.Clone();
		}

		auto result = a_array.Clone();

		auto random = REX::Random64();
		random.NextRange(result);

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> SeededArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::uint32_t a_seed)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.size() < 2) {
			return a_array.Clone();
		}

		auto result = a_array.Clone();

		auto random = REX::Random64(a_seed);
		random.NextRange(result);

		return result;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomSeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomAlphanumericChar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededAlphanumericChar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomLong);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededLong);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomDouble);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededDouble);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomAlphanumericString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededAlphanumericString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomElement);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededElement);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RandomArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SeededArray);
	}
}
