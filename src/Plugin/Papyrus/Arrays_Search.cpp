#include "Plugin/Papyrus/Arrays.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Arrays::Search
{
	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_left,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_right)
	{
		const auto comparison = a_left.Compare(a_right);
		return REX::GetNumericStrongOrdering(comparison);
	}

	static std::int32_t DeepCompare(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_left,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_right)
	{
		const auto comparison = a_left.DeepCompare(a_right);
		return REX::GetNumericPartialOrdering(comparison);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_left,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_right)
	{
		return a_left.Equals(a_right);
	}

	static bool DeepEquals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_left,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_right)
	{
		return a_left.DeepEquals(a_right);
	}

	static bool IsNone(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		return a_array == nullptr;
	}

	static bool IsEmpty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return false;
		}

		const auto arrayLock = a_array.Lock();
		return a_array.empty();
	}

	static bool IsNoneOrEmpty(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) {
			return true;
		}

		const auto arrayLock = a_array.Lock();
		return a_array.empty();
	}

	static bool Contains(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return false;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return false;
		}

		return std::ranges::any_of(a_array, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});
	}

	static bool StartsWith(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return false;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return false;
		}

		return a_array.front().Equals(RE::BSScript::UnwrapVariant(a_value));
	}

	static bool EndsWith(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return false;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return false;
		}

		return a_array.back().Equals(RE::BSScript::UnwrapVariant(a_value));
	}

	static bool Any(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array02)
	{
		if (!a_array01) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_01_NULL, a_stackId);
			return false;
		}

		if (!a_array02) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_02_NULL, a_stackId);
			return false;
		}

		if (a_array01 == a_array02) {
			return true;
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_array01.empty() || a_array02.empty()) {
			return false;
		}

		return std::ranges::any_of(a_array02, [&a_array01](const RE::BSScript::Variable& a_element02) {
			return std::ranges::any_of(a_array01, [&a_element02](const RE::BSScript::Variable& a_element01) {
				return a_element01.Equals(a_element02);
			});
		});
	}

	static bool All(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array02)
	{
		if (!a_array01) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_01_NULL, a_stackId);
			return false;
		}

		if (!a_array02) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_02_NULL, a_stackId);
			return false;
		}

		if (a_array01 == a_array02) {
			return true;
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_array01.empty() || a_array02.empty()) {
			return false;
		}

		return std::ranges::all_of(a_array02, [&a_array01](const RE::BSScript::Variable& a_element02) {
			return std::ranges::any_of(a_array01, [&a_element02](const RE::BSScript::Variable& a_element01) {
				return a_element01.Equals(a_element02);
			});
		});
	}

	static std::uint32_t Size(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0;
		}

		const auto arrayLock = a_array.Lock();
		return a_array.size();
	}

	static RE::BSScript::UniqueVariant First(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.front());
	}

	static RE::BSScript::UniqueVariant Last(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array.back());
	}

	static RE::BSScript::UniqueVariant At(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_index)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_index < 0 || std::cmp_greater_equal(a_index, a_array.size())) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, a_array.size());
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_array[a_index]);
	}

	static std::int32_t IndexOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return -1;
		}

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		a_count = std::min(a_count, size);

		const auto* varBegin = a_array.begin() + a_startIndex;
		const auto* varEnd = a_array.begin() + std::min(size, a_startIndex + a_count);

		const auto* varIt = std::find_if(varBegin, varEnd, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (varIt != varEnd) {
			return static_cast<std::int32_t>(std::distance(a_array.cbegin(), varIt));
		}

		return -1;
	}

	static std::int32_t LastIndexOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return -1;
		}

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_startIndex < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		if (a_startIndex >= size) {
			a_startIndex = size - 1;
		}

		a_count = std::min(a_count, size);

		const auto varBegin = a_array.rbegin() + size - a_startIndex - 1;
		const auto varEnd = a_array.rbegin() + std::min(size, size - a_startIndex - 1 + a_count);

		const auto varIt = std::find_if(varBegin, varEnd, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (varIt != varEnd) {
			return static_cast<std::int32_t>(std::distance(a_array.begin(), varIt.base()) - 1);
		}

		return -1;
	}

	static std::int32_t Count(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0;
		}

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return 0;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return 0;
		}

		if (a_count == 0 || size == 0) {
			return 0;
		}

		a_count = std::min(a_count, size);

		const auto* varBegin = a_array.cbegin() + a_startIndex;
		const auto* varEnd = a_array.cbegin() + std::min(size, a_startIndex + a_count);

		return static_cast<std::int32_t>(std::ranges::count_if(varBegin, varEnd, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		}));
	}

	static RE::BSScript::UniqueVariant Find(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		const auto* varIt = std::ranges::find_if(a_array.begin(), a_array.end(), [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (!varIt || varIt == a_array.end()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(varIt);
	}

	static RE::BSScript::UniqueVariant FindLast(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		const auto varIt = std::ranges::find_if(a_array | std::ranges::views::reverse, [&a_value](const RE::BSScript::Variable& a_element) {
			return a_element.Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (varIt == a_array.rend()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(varIt.base());
	}

	static RE::BSScript::UniqueVariant Min(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		const auto* varIt = std::ranges::min_element(a_array, [](const RE::BSScript::Variable& a_left, const RE::BSScript::Variable& a_right) {
			return a_left.Compare(a_right) < 0;
		});

		if (!varIt || varIt == a_array.end()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(*varIt);
	}

	static RE::BSScript::UniqueVariant Max(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		if (a_array.empty()) {
			return nullptr;
		}

		const auto* varIt = std::ranges::max_element(a_array, [](const RE::BSScript::Variable& a_left, const RE::BSScript::Variable& a_right) {
			return a_left.Compare(a_right) < 0;
		});

		if (!varIt || varIt == a_array.end()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(*varIt);
	}

	static std::int32_t SumInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<std::vector<std::int32_t>> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0;
		}

		if (a_array->empty()) {
			return 0;
		}

		return std::ranges::fold_left(a_array.value(), 0, std::plus<std::int32_t>());
	}

	static REX::Float64 SumFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<std::vector<REX::Float64>> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0.0_f64;
		}

		if (a_array->empty()) {
			return 0.0_f64;
		}

		return std::ranges::fold_left(a_array.value(), 0.0_f64, std::plus<REX::Float64>());
	}

	static std::int32_t AverageInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<std::vector<std::int32_t>> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0;
		}

		if (a_array->empty()) {
			return 0;
		}

		return std::ranges::fold_left(a_array.value(), 0, std::plus<std::int32_t>()) / static_cast<std::int32_t>(a_array->size());
	}

	static REX::Float64 AverageFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<std::vector<REX::Float64>> a_array)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return 0.0_f64;
		}

		if (a_array->empty()) {
			return 0.0_f64;
		}

		return std::ranges::fold_left(a_array.value(), 0.0_f64, std::plus<REX::Float64>()) / static_cast<REX::Float64>(a_array->size());
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Union(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
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

		if (a_array01 == a_array02) {
			return a_array01.Clone();
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_array01.empty()) {
			return a_array02.Clone();
		}

		if (a_array02.empty()) {
			return a_array01.Clone();
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(std::max(a_array01.size(), a_array02.size()));

		for (const auto& element : a_array01) {
			auto value = RE::BSScript::CreateVariant(element);
			result.elements().emplace_back(std::move(value));
		}

		for (const auto& element : a_array02) {
			const auto filter = [&element](const RE::BSScript::Variable& a_otherElement) {
				return a_otherElement.Equals(element);
			};

			if (std::ranges::none_of(result, filter)) {
				auto value = RE::BSScript::CreateVariant(element);
				result.elements().emplace_back(std::move(value));
			}
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Intersect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
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

		if (a_array01 == a_array02) {
			return a_array01.Clone();
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_array01.empty() || a_array02.empty()) {
			return {};
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(std::min(a_array01.size(), a_array02.size()));

		for (const auto& element : a_array01) {
			const auto filter = [&element](const RE::BSScript::Variable& a_otherElement) {
				return a_otherElement.Equals(element);
			};

			if (std::ranges::any_of(a_array02, filter)) {
				auto value = RE::BSScript::CreateVariant(element);
				result.elements().emplace_back(std::move(value));
			}
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Except(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array01,
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

		if (a_array01 == a_array02) {
			return {};
		}

		const auto arrayLock01 = a_array01.Lock();
		const auto arrayLock02 = a_array02.Lock();

		if (a_array01.empty() || a_array02.empty()) {
			return a_array01.Clone();
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(std::max(a_array01.size(), a_array02.size()));

		for (const auto& element : a_array01) {
			const auto filter = [&element](const RE::BSScript::Variable& a_otherElement) {
				return a_otherElement.Equals(element);
			};

			if (std::ranges::none_of(a_array02, filter)) {
				auto value = RE::BSScript::CreateVariant(element);
				result.elements().emplace_back(std::move(value));
			}
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Take(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_count < 0 || a_count > size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_OUT_OF_RANGE, a_stackId, a_count, 0, size);
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_count);

		for (auto i = 0; i < a_count; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Skip(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		std::int32_t a_count)
	{
		if (!a_array) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		const auto arrayLock = a_array.Lock();

		const auto size = static_cast<std::int32_t>(a_array.size());
		if (a_count < 0 || a_count > size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_OUT_OF_RANGE, a_stackId, a_count, 0, size);
			return nullptr;
		}

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(size - a_count);

		for (auto i = a_count; i < size; i++) {
			auto value = RE::BSScript::CreateVariant(a_array[i]);
			result.elements().emplace_back(std::move(value));
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Distinct(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
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

		auto result = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		result.elements().reserve(a_array.size());

		for (const auto& element : a_array) {
			const auto filter = [&element](const RE::BSScript::Variable& a_otherElement) {
				return a_otherElement.Equals(element);
			};

			if (std::ranges::none_of(result, filter)) {
				auto value = RE::BSScript::CreateVariant(element);
				result.elements().emplace_back(std::move(value));
			}
		}

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Sort(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_array,
		bool a_descending)
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
		std::ranges::sort(result, [a_descending](const RE::BSScript::Variable& a_left, const RE::BSScript::Variable& a_right) {
			return (a_left.Compare(a_right) < 0) != a_descending;
		});

		return result;
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> Reverse(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
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
		std::ranges::reverse(result);

		return result;
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCompare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNoneOrEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartsWith);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, EndsWith);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Any);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, All);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Size);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, First);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Last);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, At);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IndexOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, LastIndexOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Count);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Find);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FindLast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Min);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Max);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SumInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SumFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AverageInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AverageFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Union);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Intersect);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Except);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Take);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Skip);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Distinct);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Sort);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Reverse);
	}
}
