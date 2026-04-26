#include "Plugin/Papyrus/Arrays.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Arrays::Manipulation
{
	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Add(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array.size() + 1);

		for (const auto& element : a_array) {
			auto value = RE::BSScript::CreateVariant(element);
			result.elements().emplace_back(std::move(value));
		}

		auto value = RE::BSScript::CreateVariant(a_value);
		result.elements().emplace_back(std::move(value));

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> AddRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_values)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_01_NULL, a_stackId);
			return nullptr;
		}

		if (!a_values) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_02_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock01 = a_array.Lock();
		const auto arrayLock02 = a_values.Lock();

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array.size() + a_values.size());

		for (const auto& element : a_array) {
			auto value = RE::BSScript::CreateVariant(element);
			result.elements().emplace_back(std::move(value));
		}

		for (const auto& element : a_values) {
			auto value = RE::BSScript::CreateVariant(element);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Insert(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_index,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_index < 0 || std::cmp_greater(a_index, a_array.size())) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, a_array.size());
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array.size() + 1);

		for (auto i = 0; i < a_index; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		auto insertion = RE::BSScript::CreateVariant(a_value);
		result.elements().emplace_back(std::move(insertion));

		for (auto i = a_index; std::cmp_less(i, a_array.size()); i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> InsertRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
		std::int32_t a_index,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array02)
	{
		if (!a_array01) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_01_NULL, a_stackId);
			return nullptr;
		}

		if (!a_array02) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_02_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_index < 0 || std::cmp_greater(a_index, a_array01.size())) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, a_array01.size());
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array01.size() + a_array02.size());

		for (auto i = 0; i < a_index; i++) {
			auto value = RE::BSScript::CreateVariant(a_array01[i]);
			result.elements().emplace_back(std::move(value));
		}

		for (const auto& element : a_array02) {
			auto value = RE::BSScript::CreateVariant(element);
			result.elements().emplace_back(std::move(value));
		}

		for (auto i = a_index; std::cmp_less(i, a_array01.size()); i++) {
			auto value = RE::BSScript::CreateVariant(a_array01[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Remove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return {};
		}

		const auto* arrayIt = std::ranges::find_if(a_array, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array.size() - 1);

		for (auto* i = a_array.begin(); i < arrayIt; i++) {
			auto value = RE::BSScript::CreateVariant(*i);
			result.elements().emplace_back(std::move(value));
		}

		for (const auto* i = arrayIt + 1; i < a_array.end(); i++) {
			auto value = RE::BSScript::CreateVariant(*i);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RemoveAll(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return {};
		}

		const auto arrayIt = std::ranges::remove_if(a_array, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(static_cast<std::uint32_t>(std::distance(a_array.begin(), arrayIt.begin())));

		for (auto* i = a_array.begin(); i != arrayIt.begin(); i++) {
			auto value = RE::BSScript::CreateVariant(*i);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RemoveAt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_index)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return nullptr;
		}

		if (size == 0) {
			return {};
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(size - 1);

		for (auto i = 0; i < a_index; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		for (auto i = a_index + 1; i < size; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> RemoveRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_index,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return nullptr;
		}

		if (a_count < 0 || a_count > size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_OUT_OF_RANGE, a_stackId, a_count, 0, size);
			return nullptr;
		}

		if (size == 0) {
			return {};
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(size - a_count);

		for (auto i = 0; i < a_index; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		for (auto i = a_index + a_count; i < size; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Fill(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return nullptr;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		if (size == 0) {
			return {};
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(size);

		for (auto i = 0; i < a_startIndex; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		for (auto i = 0; i < a_count; i++) {
			auto value = RE::BSScript::CreateVariant(a_value);
			result.elements().emplace_back(std::move(value));
		}

		for (auto i = a_startIndex + a_count; i < size; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Resize(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			auto value = std::cmp_less(i, a_array.size()) ? RE::BSScript::CreateVariant(a_array[i]) : nullptr;
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static std::string ToString(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) {
			return std::string(RE::BSScript::NONE_STRING);
		}

		return a_array->ToString();
	}

	static std::string Join(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::string_view a_separator)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return {};
		}

		const auto arrayLock = a_array.Lock();

		auto result = std::string();
		for (const auto* arrayIt = a_array.begin(); arrayIt != a_array.end(); arrayIt++) {
			result += arrayIt->ToString();
			if (arrayIt != a_array.cend() - 1) {
				result += a_separator;
			}
		}

		return result;
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Add);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AddRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Insert);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, InsertRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Remove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveAll);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveAt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Fill);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Resize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Join);
	}
}
