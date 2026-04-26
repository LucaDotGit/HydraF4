#pragma once

namespace Plugin::Internal::Core::TomlUtility
{
	struct type_config final
	{
	public:
		using comment_type = toml::preserve_comments;

		using boolean_type = bool;
		using integer_type = std::int64_t;
		using floating_type = REX::Float64;
		using string_type = std::string;

		template <typename T>
		using array_type = std::vector<T>;
		template <typename TKey, typename TValue>
		using table_type = std::map<TKey, TValue, REX::LessIgnoreCase>;

		[[nodiscard]] static toml::result<integer_type, toml::error_info> parse_int(const std::string& a_str, const toml::source_location& a_src, std::uint8_t a_base)
		{
			return toml::read_int<integer_type>(a_str, a_src, a_base);
		}

		[[nodiscard]] static toml::result<floating_type, toml::error_info> parse_float(const std::string& a_str, const toml::source_location& a_src, bool a_isHex)
		{
			return toml::read_float<floating_type>(a_str, a_src, a_isHex);
		}
	};

	using toml_t = toml::basic_value<type_config>;

	inline constexpr auto TABLE_SEPARATOR = REX::Toml::TABLE_SEPARATOR;
	inline constexpr auto KEY_SEPARATOR = REX::Toml::KEY_SEPARATOR;

	inline constexpr auto SPECIFICATION_VERSION = REX::Toml::SPECIFICATION_VERSION;

	[[nodiscard]] auto ToInt(const toml_t& a_value) -> std::optional<std::int32_t>;
	[[nodiscard]] auto ToUInt(const toml_t& a_value) -> std::optional<std::uint32_t>;
	[[nodiscard]] auto ToFloat(const toml_t& a_value) -> std::optional<REX::Float32>;
	[[nodiscard]] auto ToDouble(const toml_t& a_value) -> std::optional<REX::Float64>;
	[[nodiscard]] auto ToString(const toml_t& a_value) -> std::optional<std::string>;

	[[nodiscard]] auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value) -> std::optional<toml_t>;
	[[nodiscard]] auto ToArray(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>;

	[[nodiscard]] auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<toml_t>;
	[[nodiscard]] auto ToVar(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>;

	[[nodiscard]] auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<toml_t>;
	[[nodiscard]] auto ToValue(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>;

	[[nodiscard]] auto ToDetectedValue(const toml_t& a_value) -> std::optional<RE::BSScript::Variable>;
}
