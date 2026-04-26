#pragma once

#include "Plugin/Internal/Core/StringUtility.hpp"

namespace Plugin::Internal::Core::StringUtility
{
	constexpr auto FormatUnexpected(std::string a_format) -> std::unexpected<std::string>
	{
		return std::unexpected(std::move(a_format));
	}

	constexpr auto FormatUnexpected(std::string_view a_format) -> std::unexpected<std::string>
	{
		return std::unexpected(std::string(a_format));
	}

	template <class... Args>
	constexpr auto FormatUnexpected(REX::FormatString<Args...> a_format, Args&&... a_args) -> std::unexpected<std::string>
		requires((REX::formattable<Args> && ...))
	{
		return std::unexpected(REX::Format(a_format, std::forward<Args>(a_args)...));
	}
}
