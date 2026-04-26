#include "Plugin/Papyrus/Strings.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Strings::Manipulation
{
	static std::string Substring(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return {};
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return {};
		}

		if (a_count == 0) {
			return {};
		}

		return std::string(a_value.substr(a_startIndex, a_count));
	}

	static std::string Remove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return {};
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return {};
		}

		if (a_count == 0) {
			return std::string(a_value);
		}

		return std::string(a_value).erase(a_startIndex, a_count);
	}

	static std::string Insert(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_index,
		std::string_view a_substring)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return {};
		}

		if (a_substring.empty()) {
			return std::string(a_value);
		}

		return std::string(a_value).insert(a_index, a_substring);
	}

	static std::string Replace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_substring,
		std::string_view a_replacement,
		std::int32_t a_count)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return {};
		}

		if (a_count == 0 || a_substring.empty()) {
			return std::string(a_value);
		}

		const auto comparer = [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		};

		auto result = std::string(a_value);
		auto i = static_cast<std::size_t>(0);

		for (; a_count > 0; a_count--) {
			const auto charIt = std::search(result.begin() + static_cast<std::ptrdiff_t>(i), result.end(), a_substring.begin(), a_substring.end(), comparer);
			if (charIt == result.end()) {
				break;
			}

			const auto position = std::distance(result.begin(), charIt);
			result.replace(position, a_substring.size(), a_replacement);
			i = position + a_replacement.size();
		}

		return result;
	}

	static std::string ReplaceAt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_index,
		std::string_view a_replacement)
	{
		const auto size = static_cast<std::int32_t>(a_value.size());
		if (a_index < 0 || a_index >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, size);
			return {};
		}

		if (a_replacement.empty()) {
			return std::string(a_value);
		}

		return std::string(a_value).replace(a_index, a_replacement.size(), a_replacement);
	}

	static std::optional<std::vector<std::string>> Split(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_separator,
		std::int32_t a_count)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return std::nullopt;
		}

		if (a_count == 0 || a_separator.empty()) {
			return std::vector<std::string>{ std::string(a_value) };
		}

		const auto comparer = [](char a_left, char a_right) noexcept {
			return REX::ToLower(a_left) == REX::ToLower(a_right);
		};

		auto i = static_cast<std::size_t>(0);
		auto result = std::vector<std::string>();

		for (; a_count > 1; a_count--) {
			const auto charIt = std::search(a_value.begin() + static_cast<std::ptrdiff_t>(i), a_value.end(), a_separator.begin(), a_separator.end(), comparer);
			if (charIt == a_value.end()) {
				break;
			}

			const auto position = std::distance(a_value.begin(), charIt);
			result.emplace_back(a_value.substr(i, position - i));
			i = position + a_separator.size();
		}

		result.emplace_back(a_value.substr(i));
		return result;
	}

	static std::string Join(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSFixedString> a_values,
		std::string_view a_separator)
	{
		auto result = std::string();
		for (auto valueIt = a_values.begin(); valueIt != a_values.end(); valueIt++) {
			result += *valueIt;
			if (valueIt != a_values.end() - 1) {
				result += a_separator;
			}
		}

		return result;
	}

	static std::string Concat(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSFixedString> a_values)
	{
		auto result = std::string();
		for (const auto& string : a_values) {
			result += string;
		}

		return result;
	}

	static std::string Repeat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_count)
	{
		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return {};
		}

		if (a_count == 0) {
			return {};
		}

		auto result = std::string();
		result.reserve(a_value.size() * a_count);

		for (auto i = 0; i < a_count; i++) {
			result += a_value;
		}

		return result;
	}

	static std::string Reverse(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value)
	{
		auto result = std::string(a_value);
		std::ranges::reverse(result);
		return result;
	}

	static std::string Trim(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_trimString)
	{
		if (a_value.empty() || a_trimString.empty()) {
			return std::string(a_value);
		}

		const auto filter = [a_trimString](char a_left) noexcept {
			return std::ranges::any_of(a_trimString, [a_left](char a_right) noexcept {
				return REX::ToLower(a_left) == REX::ToLower(a_right);
			});
		};

		const auto charBegin = std::ranges::find_if_not(a_value, filter);
		const auto charEnd = std::ranges::find_if_not(a_value | std::ranges::views::reverse, filter);

		return { charBegin, charEnd.base() };
	}

	static std::string TrimStart(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_trimString)
	{
		if (a_value.empty() || a_trimString.empty()) {
			return std::string(a_value);
		}

		const auto charBegin = std::ranges::find_if_not(a_value, [a_trimString](char a_left) noexcept {
			return std::ranges::any_of(a_trimString, [a_left](char a_right) noexcept {
				return REX::ToLower(a_left) == REX::ToLower(a_right);
			});
		});

		return { charBegin, a_value.end() };
	}

	static std::string TrimEnd(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::string_view a_trimString)
	{
		if (a_value.empty() || a_trimString.empty()) {
			return std::string(a_value);
		}

		const auto charEnd = std::ranges::find_if_not(a_value | std::ranges::views::reverse, [a_trimString](char a_left) noexcept {
			return std::ranges::any_of(a_trimString, [a_left](char a_right) noexcept {
				return REX::ToLower(a_left) == REX::ToLower(a_right);
			});
		});

		return { a_value.begin(), charEnd.base() };
	}

	static std::string PadLeft(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_totalSize,
		std::string_view a_padString)
	{
		if (a_totalSize < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TOTAL_SIZE_NEGATIVE, a_stackId);
			return {};
		}

		if (a_padString.empty()) {
			return std::string(a_value);
		}

		const auto padSize = a_totalSize - static_cast<std::int32_t>(a_value.size());
		if (padSize <= 0) {
			return std::string(a_value);
		}

		const auto padCount = padSize / a_padString.size();
		const auto padRemainder = padSize % a_padString.size();

		auto padding = std::string();
		padding.reserve(padSize);

		for (auto i = 0ui32; i < padCount; i++) {
			padding += a_padString;
		}

		padding += a_padString.substr(0, padRemainder);
		return padding + std::string(a_value);
	}

	static std::string PadRight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_totalSize,
		std::string_view a_padString)
	{
		if (a_totalSize < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TOTAL_SIZE_NEGATIVE, a_stackId);
			return {};
		}

		if (a_padString.empty()) {
			return std::string(a_value);
		}

		const auto padSize = a_totalSize - static_cast<std::int32_t>(a_value.size());
		if (padSize <= 0) {
			return std::string(a_value);
		}

		const auto padCount = padSize / a_padString.size();
		const auto padRemainder = padSize % a_padString.size();

		auto padding = std::string();
		padding.reserve(padSize);

		for (auto i = 0ui32; i < padCount; i++) {
			padding += a_padString;
		}

		padding += a_padString.substr(0, padRemainder);
		return std::string(a_value) + padding;
	}

	static std::string Truncate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_value,
		std::int32_t a_totalSize,
		std::string_view a_suffix)
	{
		if (a_totalSize < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TOTAL_SIZE_NEGATIVE, a_stackId);
			return {};
		}

		if (a_suffix.empty()) {
			return std::string(a_value);
		}

		const auto size = static_cast<std::int32_t>(a_value.size());
		if (size <= a_totalSize) {
			return std::string(a_value);
		}

		return std::string(a_value.substr(0, a_totalSize)) + std::string(a_suffix);
	}

	static std::string Format(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_format,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		std::string_view a_default)
	{
		auto result = ::Plugin::Internal::Core::StringUtility::Format(a_args, a_format);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().what(), a_stackId);
			return std::string(a_default);
		}

		return std::move(result).value();
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Substring);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Remove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Insert);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Replace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ReplaceAt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Split);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Join);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Concat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Repeat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Reverse);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Trim);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TrimStart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TrimEnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PadLeft);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PadRight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Truncate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Format);
	}
}
