#pragma once

#include "Plugin/Internal/Core/TypeSerializer.hpp"

namespace Plugin::Internal::Core::JsonUtility
{
	template <class Key, class T, class Comparer = REX::LessIgnoreCase,
		class TAllocator = std::allocator<std::pair<const Key, T>>>
	using json_map_t = std::map<Key, T, REX::LessIgnoreCase, TAllocator>;

	using json_t = nlohmann::basic_json<
		json_map_t,
		std::vector,
		std::string,
		bool,
		std::int64_t,
		std::uint64_t,
		REX::Float64>;

	inline constexpr auto KEY_SEPARATOR = REX::Json::KEY_SEPARATOR;

	inline constexpr auto ALLOW_EXCEPTIONS = REX::Json::ALLOW_EXCEPTIONS;
	inline constexpr auto IGNORE_COMMENTS = REX::Json::IGNORE_COMMENTS;
	inline constexpr auto ENSURE_ASCII = REX::Json::ENSURE_ASCII;

	[[nodiscard]] const json_t::parser_callback_t& GetParserCallback() noexcept;
	inline constexpr auto ERROR_HANDLER = REX::Json::ERROR_HANDLER<json_t>;

	inline constexpr auto INDENT_COUNT = REX::Json::INDENT_COUNT;
	inline constexpr auto INDENT_CHAR = REX::Json::INDENT_CHAR;

	[[nodiscard]] auto ToInt(const json_t& a_value) -> std::optional<std::int32_t>;
	[[nodiscard]] auto ToUInt(const json_t& a_value) -> std::optional<std::uint32_t>;
	[[nodiscard]] auto ToFloat(const json_t& a_value) -> std::optional<REX::Float32>;
	[[nodiscard]] auto ToDouble(const json_t& a_value) -> std::optional<REX::Float64>;

	[[nodiscard]] auto FromFormData(const Core::TypeSerializer::FormData& a_value) -> json_t;
	[[nodiscard]] auto ToFormData(const json_t& a_value) -> std::optional<Core::TypeSerializer::FormData>;

	[[nodiscard]] auto FromAliasData(const Core::TypeSerializer::AliasData& a_value) -> json_t;
	[[nodiscard]] auto ToAliasData(const json_t& a_value) -> std::optional<Core::TypeSerializer::AliasData>;

	[[nodiscard]] auto FromInventoryRefData(const Core::TypeSerializer::InventoryRefData& a_value) -> json_t;
	[[nodiscard]] auto ToInventoryRefData(const json_t& a_value) -> std::optional<Core::TypeSerializer::InventoryRefData>;

	[[nodiscard]] auto FromActiveEffectData(const Core::TypeSerializer::ActiveEffectData& a_value) -> json_t;
	[[nodiscard]] auto ToActiveEffectData(const json_t& a_value) -> std::optional<Core::TypeSerializer::ActiveEffectData>;

	[[nodiscard]] auto FromInputLayerData(const Core::TypeSerializer::InputLayerData& a_value) -> json_t;
	[[nodiscard]] auto ToInputLayerData(const json_t& a_value) -> std::optional<Core::TypeSerializer::InputLayerData>;

	[[nodiscard]] auto FromForm(const Core::TypeSerializer::FormVariant& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToForm(const json_t& a_value) -> std::optional<Core::TypeSerializer::FormVariant>;

	[[nodiscard]] auto FromAlias(const RE::BGSBaseAlias* a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToAlias(const json_t& a_value) -> std::optional<RE::BGSBaseAlias*>;

	[[nodiscard]] auto FromInventoryRef(const Core::TypeSerializer::InventoryItemRef& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToInventoryRef(const json_t& a_value) -> std::optional<Core::TypeSerializer::InventoryItemRef>;

	[[nodiscard]] auto FromActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToActiveEffect(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>;

	[[nodiscard]] auto FromInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToInputLayer(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>;

	[[nodiscard]] auto FromObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToObject(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>;

	[[nodiscard]] auto FromStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToStruct(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>;

	[[nodiscard]] auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToArray(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>;

	[[nodiscard]] auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToVar(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>;

	[[nodiscard]] auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<json_t>;
	[[nodiscard]] auto ToValue(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>;

	[[nodiscard]] auto ToDetectedValue(const json_t& a_value) -> std::optional<RE::BSScript::Variable>;

	[[nodiscard]] auto GetPointer(std::string_view a_key) -> std::expected<json_t::json_pointer, json_t::exception>;
}
