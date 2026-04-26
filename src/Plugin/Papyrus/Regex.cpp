#include "Plugin/Papyrus/Regex.hpp"

#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Regex::Impl
{
	static constexpr auto DEFAULT_FLAGS =
		static_cast<boost::regex_constants::flag_type_>(
			boost::regex_constants::perl |
			boost::regex_constants::icase |
			boost::regex_constants::match_perl |
			boost::regex_constants::match_single_line |
			boost::regex_constants::match_not_dot_newline);

	static constexpr auto ESCAPE_PREFIX = '\\';
	static constexpr auto ESCAPE_CHARS = R"(.^$*+?()[{\|# )"sv;

	[[nodiscard]] static constexpr auto GetFlags(std::string_view a_flags) noexcept
		-> boost::regex_constants::flag_type_
	{
		auto flagSet = REX::EnumSet(DEFAULT_FLAGS);

		for (const auto character : a_flags) {
			switch (character) {
				case 'm':
				case 'M': {
					flagSet.reset(boost::regex_constants::match_single_line);
					continue;
				}
				case 's':
				case 'S': {
					flagSet.reset(boost::regex_constants::match_not_dot_newline);
					continue;
				}
				[[unlikely]] default: {
					continue;
				}
			}
		}

		return flagSet.get();
	}

	[[nodiscard]] __forceinline static constexpr auto Escape(std::string_view a_string) -> std::string
	{
		auto result = std::string();
		result.reserve(a_string.size());

		for (const auto character : a_string) {
			if (ESCAPE_CHARS.contains(character)) {
				result.push_back(ESCAPE_PREFIX);
			}

			result.push_back(character);
		}

		return result;
	}

	[[nodiscard]] __forceinline static constexpr auto Unescape(std::string_view a_string) -> std::string
	{
		auto result = std::string();
		result.reserve(a_string.size());

		for (auto charIt = a_string.begin(); charIt != a_string.end(); charIt++) {
			const auto character = *charIt;
			if (character != ESCAPE_PREFIX) {
				result.push_back(character);
				continue;
			}

			charIt = std::next(charIt);
			if (charIt == a_string.end()) {
				break;
			}

			const auto nextChar = *charIt;
			if (ESCAPE_CHARS.contains(nextChar)) {
				result.push_back(nextChar);
				continue;
			}

			result.push_back(ESCAPE_PREFIX);
			result.push_back(nextChar);
		}

		return result;
	}

	static_assert(Unescape(Escape(ESCAPE_CHARS)) == ESCAPE_CHARS);
}

namespace Plugin::Papyrus::Regex
{
	static bool IsValid(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_flags)
	{
		try {
			std::ignore = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));
			return true;
		}
		catch ([[maybe_unused]] const boost::regex_error& error) {
			return false;
		}
	}

	static bool IsMatch(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));
			return boost::regex_search(a_string.begin(), a_string.end(), regex);
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return false;
		}
	}

	static std::string Escape(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string)
	{
		return Impl::Escape(a_string);
	}

	static std::string Unescape(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string)
	{
		return Impl::Unescape(a_string);
	}

	static std::string Match(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));

			auto match = boost::match_results<std::string_view::const_iterator>();
			if (!boost::regex_match(a_string.begin(), a_string.end(), match, regex)) {
				return {};
			}

			return match.str();
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return {};
		}
	}

	static std::optional<std::vector<std::string>> Matches(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));
			const auto matchBegin = boost::regex_iterator(a_string.begin(), a_string.end(), regex);
			const auto matchEnd = boost::regex_iterator<std::string_view::const_iterator>();

			auto matches = std::vector<std::string>();
			for (auto matchIt = matchBegin; matchIt != matchEnd; matchIt++) {
				for (const auto& match : *matchIt) {
					matches.push_back(match.str());
				}
			}

			return matches;
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return std::nullopt;
		}
	}

	static std::int32_t Search(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));

			auto match = boost::match_results<std::string_view::const_iterator>();
			if (!boost::regex_search(a_string.begin(), a_string.end(), match, regex)) {
				return -1;
			}

			return static_cast<std::int32_t>(match.position());
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return -1;
		}
	}

	static std::string Replace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_replacement,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));

			auto result = std::string();
			boost::regex_replace(std::back_inserter(result), a_string.begin(), a_string.end(), regex, a_replacement, boost::format_first_only);

			return result;
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return {};
		}
	}

	static std::string ReplaceAll(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::string_view a_replacement,
		std::string_view a_flags)
	{
		try {
			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));

			auto result = std::string();
			boost::regex_replace(std::back_inserter(result), a_string.begin(), a_string.end(), regex, a_replacement);

			return result;
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return {};
		}
	}

	static std::optional<std::vector<std::string>> Split(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_string,
		::Plugin::Internal::Script::ScriptStrictString a_pattern,
		std::int32_t a_count,
		std::string_view a_flags)
	{
		try {
			constexpr auto SUBMATCH_ALL = -1;

			const auto regex = boost::regex(a_pattern.begin(), a_pattern.end(), Impl::GetFlags(a_flags));
			const auto tokenBegin = boost::regex_token_iterator(a_string.begin(), a_string.end(), regex, SUBMATCH_ALL);
			const auto tokenEnd = boost::regex_token_iterator<std::string_view::const_iterator>();

			auto tokens = std::vector<std::string>();
			for (auto tokenIt = tokenBegin; tokenIt != tokenEnd && a_count > 0; tokenIt++, a_count--) {
				tokens.push_back(tokenIt->str());
			}

			return tokens;
		}
		catch (const boost::regex_error& error) {
			a_vm.PostError(error.what(), a_stackId);
			return std::nullopt;
		}
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsValid);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsMatch);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Escape);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Unescape);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Match);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Matches);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Search);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Replace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ReplaceAll);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Split);
	}
}
