#pragma once

namespace Plugin::Internal::Core::StringUtility
{
	[[nodiscard]] auto Format(std::span<const RE::BSScript::Variable> a_values, std::string_view a_format) -> std::expected<std::string, fmt::format_error>;

	[[nodiscard]] __forceinline constexpr auto FormatUnexpected(std::string a_format) -> std::unexpected<std::string>;
	[[nodiscard]] __forceinline constexpr auto FormatUnexpected(std::string_view a_format) -> std::unexpected<std::string>;

	template <class... Args>
	[[nodiscard]] __forceinline constexpr auto FormatUnexpected(REX::FormatString<Args...> a_format, Args&&... a_args) -> std::unexpected<std::string>
		requires((REX::formattable<Args> && ...));
}

#include "Plugin/Internal/Core/StringUtility.inl"
