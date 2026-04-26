#include "Plugin/Internal/Core/StringUtility.hpp"

namespace Plugin::Internal::Core::StringUtility
{
	auto Format(std::span<const RE::BSScript::Variable> a_values, std::string_view a_format) -> std::expected<std::string, fmt::format_error>
	{
		if (a_values.empty() || a_format.empty()) {
			return std::string();
		}

		auto args = fmt::dynamic_format_arg_store<fmt::format_context>();
		args.reserve(a_values.size(), a_values.size());

		for (const auto& arg : a_values) {
			args.push_back(arg.ToString());
		}

		try {
			return fmt::vformat(a_format, args);
		}
		catch (const fmt::format_error& error) {
			return std::unexpected(error);
		}
	}
}
