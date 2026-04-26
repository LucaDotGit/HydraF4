#include "Plugin/Papyrus/Strings.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Strings::Search
{
	static RE::BSFixedString Empty(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return {};
	}

	static bool IsEmpty(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return a_value.empty();
	}

	static bool IsAscii(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsAscii(a_char);
		});
	}

	static bool IsWhiteSpace(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsWhiteSpace(a_char);
		});
	}

	static bool IsBlank(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsBlank(a_char);
		});
	}

	static bool IsDigit(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsDigit(a_char);
		});
	}

	static bool IsLetter(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsLetter(a_char);
		});
	}

	static bool IsLetterOrDigit(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsLetterOrDigit(a_char);
		});
	}

	static bool IsHexadecimal(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsHexadecimal(a_char);
		});
	}

	static bool IsPunctuation(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsPunctuation(a_char);
		});
	}

	static bool IsControl(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsControl(a_char);
		});
	}

	static bool IsGraphic(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsGraphic(a_char);
		});
	}

	static bool IsPrintable(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsPrintable(a_char);
		});
	}

	static bool IsLower(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsLower(a_char);
		});
	}

	static bool IsUpper(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return std::ranges::all_of(a_value, [](char a_char) noexcept {
			return REX::IsUpper(a_char);
		});
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_left,
		RE::BSFixedString a_right)
	{
		return REX::GetNumericStrongOrdering(a_left <=> a_right);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_left,
		RE::BSFixedString a_right)
	{
		return a_left == a_right;
	}

	static bool Contains(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring)
	{
		if (a_substring.empty()) {
			return true;
		}

		return std::ranges::contains_subrange(a_value, a_substring, [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		});
	}

	static bool StartsWith(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring)
	{
		if (a_substring.empty()) {
			return true;
		}

		return std::ranges::starts_with(a_value, a_substring, [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		});
	}

	static bool EndsWith(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring)
	{
		if (a_substring.empty()) {
			return true;
		}

		return std::ranges::ends_with(a_value, a_substring, [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		});
	}

	static bool Any(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::vector<RE::BSFixedString> a_substrings)
	{
		if (a_value.empty() || a_substrings.empty()) {
			return false;
		}

		return std::ranges::any_of(a_substrings, [a_value](std::string_view a_substring) {
			return std::ranges::contains_subrange(a_value, a_substring, [](char a_left, char a_right) noexcept {
				return REX::ToLower(a_left) == REX::ToLower(a_right);
			});
		});
	}

	static bool All(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::vector<RE::BSFixedString> a_substrings)
	{
		if (a_value.empty() || a_substrings.empty()) {
			return false;
		}

		return std::ranges::all_of(a_substrings, [a_value](std::string_view a_substring) {
			return std::ranges::contains_subrange(a_value, a_substring, [](char a_left, char a_right) noexcept {
				return REX::ToLower(a_left) == REX::ToLower(a_right);
			});
		});
	}

	static std::int32_t Size(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		return static_cast<std::int32_t>(a_value.size());
	}

	static std::string First(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		if (a_value.empty()) {
			return {};
		}

		return { a_value.front() };
	}

	static std::string Last(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		if (a_value.empty()) {
			return {};
		}

		return { a_value.back() };
	}

	static std::string CharAt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_index)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return {};
		}

		return { a_value[a_index] };
	}

	static std::string FromCharArray(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::string> a_charArray)
	{
		auto result = std::string();
		for (const auto& character : a_charArray) {
			result += character;
		}

		return result;
	}

	static std::vector<std::string> ToCharArray(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		if (a_value.empty()) {
			return {};
		}

		auto result = std::vector<std::string>();
		result.reserve(a_value.size());

		for (const auto character : a_value) {
			result.emplace_back(1, character);
		}

		return result;
	}

	static char CharCodeAt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_index)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return 0;
		}

		return a_value[a_index];
	}

	static std::string FromCharCodeArray(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<char> a_charCodeArray)
	{
		auto result = std::string();
		result.reserve(a_charCodeArray.size());

		for (const auto character : a_charCodeArray) {
			result += character;
		}

		return result;
	}

	static std::vector<char> ToCharCodeArray(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		if (a_value.empty()) {
			return {};
		}

		auto result = std::vector<char>();
		result.reserve(a_value.size());

		for (const auto character : a_value) {
			result.push_back(character);
		}

		return result;
	}

	static std::int32_t IndexOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_substring.empty()) {
			return a_startIndex;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		a_count = std::min(a_count, size);

		const auto charBegin = a_value.begin() + a_startIndex;
		const auto charEnd = a_value.begin() + std::min(size, a_startIndex + a_count);

		const auto charIt = std::search(charBegin, charEnd, a_substring.begin(), a_substring.end(), [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		});

		return charIt != charEnd ? static_cast<std::int32_t>(std::distance(a_value.begin(), charIt)) : -1;
	}

	static std::int32_t IndexOfAny(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::vector<std::string> a_substrings,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_substrings.empty()) {
			return a_startIndex;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		a_count = std::min(a_count, size);

		const auto charBegin = a_value.begin() + a_startIndex;
		const auto charEnd = a_value.begin() + std::min(size, a_startIndex + a_count);

		const auto comparer = [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		};

		for (const auto& substring : a_substrings) {
			const auto charIt = std::search(charBegin, charEnd, substring.begin(), substring.end(), comparer);
			if (charIt != charEnd) {
				return static_cast<std::int32_t>(std::distance(a_value.begin(), charIt));
			}
		}

		return -1;
	}

	static std::int32_t LastIndexOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_substring.empty()) {
			return size;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		if (a_startIndex >= size) {
			a_startIndex = size - 1;
		}

		a_count = std::min(a_count, size);

		const auto charBegin = a_value.rbegin() + size - a_startIndex - 1;
		const auto charEnd = a_value.rbegin() + std::min(size, size - a_startIndex - 1 + a_count);

		const auto charIt = std::search(charBegin, charEnd, a_substring.rbegin(), a_substring.rend(), [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		});

		return charIt != charEnd ? static_cast<std::int32_t>(std::distance(a_value.begin(), charIt.base()) - a_substring.size()) : -1;
	}

	static std::int32_t LastIndexOfAny(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::vector<std::string> a_substrings,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_substrings.empty()) {
			return size;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		if (a_startIndex >= size) {
			a_startIndex = size - 1;
		}

		a_count = std::min(a_count, size);

		const auto charBegin = a_value.rbegin() + size - a_startIndex - 1;
		const auto charEnd = a_value.rbegin() + std::min(size, size - a_startIndex - 1 + a_count);

		const auto comparer = [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		};

		for (const auto& substring : a_substrings) {
			const auto charIt = std::search(charBegin, charEnd, substring.rbegin(), substring.rend(), comparer);
			if (charIt != charEnd) {
				return static_cast<std::int32_t>(std::distance(a_value.begin(), charIt.base()) - substring.size());
			}
		}

		return -1;
	}

	static std::int32_t Count(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_substring.empty()) {
			return a_count;
		}

		if (a_count == 0 || size == 0) {
			return 0;
		}

		a_count = std::min(a_count, size);

		auto charBegin = a_value.begin() + a_startIndex;
		auto charEnd = a_value.begin() + std::min(size, a_startIndex + a_count);
		auto result = 0;

		const auto comparer = [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		};

		for (; (charBegin = std::search(charBegin, charEnd, a_substring.begin(), a_substring.end(), comparer)) != charEnd; result++) {
			charBegin += static_cast<std::int32_t>(a_substring.size());
		}

		return result;
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Empty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsAscii);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsWhiteSpace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsBlank);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsDigit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLetter);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLetterOrDigit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsHexadecimal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPunctuation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsControl);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsGraphic);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPrintable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLower);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsUpper);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartsWith);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, EndsWith);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Any);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, All);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Size);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, First);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Last);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CharAt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromCharArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToCharArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CharCodeAt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, FromCharCodeArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ToCharCodeArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IndexOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IndexOfAny);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, LastIndexOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, LastIndexOfAny);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Count);
	}
}
