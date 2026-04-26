#include "Plugin/Papyrus/Arrays.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Arrays::Creation::Impl
{
	template <class T>
	[[nodiscard]] __forceinline static std::optional<std::vector<T>> Repeat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		std::int32_t a_count,
		const T& a_value)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<T>();
		result.resize(a_count, a_value);
		return result;
	}
}

namespace Plugin::Papyrus::Arrays::Creation
{
	static std::int32_t GetMaxSize(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return RE::BSScript::Array::INTERNAL_MAX_SIZE;
	}

	static std::optional<std::vector<RE::BSScript::SharedVariant>> Repeat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, static_cast<RE::BSScript::SharedVariant>(nullptr));
	}

	static std::optional<std::vector<bool>> RepeatBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		bool a_value)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, a_value);
	}

	static std::optional<std::vector<std::int32_t>> RepeatInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		std::int32_t a_value)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, a_value);
	}

	static std::optional<std::vector<REX::Float32>> RepeatFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		REX::Float32 a_value)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, a_value);
	}

	static std::optional<std::vector<RE::BSFixedString>> RepeatString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSFixedString a_value)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, a_value);
	}

	static std::optional<std::vector<RE::BSTSmartPointer<RE::BSScript::Object>>> RepeatObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSTSmartPointer<RE::BSScript::Object> a_value)
	{
		return Impl::Repeat(a_vm, a_stackId, a_count, a_value);
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RepeatVar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSScript::SharedVariant a_value)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			if (!a_value) {
				result.elements().push_back(RE::BSScript::Variable(static_cast<RE::BSScript::Variable*>(nullptr)));
				continue;
			}

			auto variant = RE::BSScript::CreateVariant(a_value->Clone(a_vm));
			result.elements().emplace_back(std::move(variant));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> DeepRepeatVar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSScript::SharedVariant a_value)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			if (!a_value) {
				result.elements().push_back(RE::BSScript::Variable(static_cast<RE::BSScript::Variable*>(nullptr)));
				continue;
			}

			auto variant = RE::BSScript::CreateVariant(a_value->DeepClone(a_vm));
			result.elements().emplace_back(std::move(variant));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RepeatArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			auto variant = RE::BSScript::CreateVariant(a_array->Clone(a_vm));
			result.elements().emplace_back(std::move(variant));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> DeepRepeatArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_count,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			auto variant = RE::BSScript::CreateVariant(a_array->DeepClone(a_vm));
			result.elements().emplace_back(std::move(variant));
		}

		return result;
	}

	static std::optional<std::vector<std::int32_t>> Range(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_start,
		std::int32_t a_count,
		std::int32_t a_step)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<std::int32_t>();
		result.reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			const auto value = a_start + (i * a_step);
			result.push_back(value);
		}

		return result;
	}

	static std::optional<std::vector<REX::Float64>> RangeFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float64 a_start,
		std::int32_t a_count,
		REX::Float64 a_step)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<REX::Float64>();
		result.reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			const auto value = a_start + (static_cast<REX::Float64>(i) * a_step);
			result.push_back(value);
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Copy(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return a_array.Clone();
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> DeepCopy(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return a_array.DeepClone();
	}

	static RE::BSScript::UniqueVariant BoolArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<bool> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::UniqueVariant IntArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<std::int32_t> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::UniqueVariant FloatArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<REX::Float32> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::UniqueVariant StringArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSFixedString> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::UniqueVariant ObjectArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSTSmartPointer<RE::BSScript::Object>> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::UniqueVariant VarArrayAsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.get());
	}

	static RE::BSScript::ArrayWrapper<bool> VarAsBoolArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsBoolArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<bool>(array);
	}

	static RE::BSScript::ArrayWrapper<std::int32_t> VarAsIntArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsIntArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<std::int32_t>(array);
	}

	static RE::BSScript::ArrayWrapper<REX::Float32> VarAsFloatArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsFloatArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<REX::Float32>(array);
	}

	static RE::BSScript::ArrayWrapper<RE::BSFixedString> VarAsStringArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsStringArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<RE::BSFixedString>(array);
	}

	static RE::BSScript::ArrayWrapper<RE::BSTSmartPointer<RE::BSScript::Object>> VarAsObjectArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsObjectArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<RE::BSTSmartPointer<RE::BSScript::Object>>(array);
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> VarAsVarArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_value || !a_value->IsVarArray()) {
			return nullptr;
		}

		const auto array = a_value->GetArray();
		return RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>(array);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaxSize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Repeat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepRepeatVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RepeatArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepRepeatArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Range);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RangeFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCopy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, BoolArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IntArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FloatArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StringArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ObjectArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarArrayAsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsBoolArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsIntArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsFloatArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsStringArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsObjectArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, VarAsVarArray);
	}
}
