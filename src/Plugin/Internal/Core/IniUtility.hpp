#pragma once

namespace Plugin::Internal::Core::IniUtility
{
	using value_t = REX::Ini::VariantType<>;
	using ini_t = CSimpleIniA;

	inline constexpr auto SETTING_SEPARATOR = REX::Ini::SETTING_SEPARATOR;

	inline constexpr auto ALLOW_KEY_ONLY = REX::Ini::ALLOW_KEY_ONLY;
	inline constexpr auto USE_UNICODE = REX::Ini::USE_UNICODE;
	inline constexpr auto USE_QUOTES = REX::Ini::USE_QUOTES;
	inline constexpr auto PREPEND_BOM = REX::Ini::PREPEND_BOM;

	void SetupFile(ini_t& a_file);

	[[nodiscard]] auto ToInt(const value_t& a_value) -> std::optional<std::int32_t>;
	[[nodiscard]] auto ToUInt(const value_t& a_value) -> std::optional<std::uint32_t>;
	[[nodiscard]] auto ToFloat(const value_t& a_value) -> std::optional<REX::Float32>;
	[[nodiscard]] auto ToDouble(const value_t& a_value) -> std::optional<REX::Float64>;

	[[nodiscard]] auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<value_t>;
	[[nodiscard]] auto ToVar(const value_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>;

	[[nodiscard]] auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<value_t>;
	[[nodiscard]] auto ToValue(const value_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>;

	[[nodiscard]] auto ToDetectedValue(const value_t& a_value) -> std::optional<RE::BSScript::Variable>;
}
