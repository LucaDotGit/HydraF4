#pragma once

#include "Plugin/Internal/Core/TypeSerializer.hpp"

namespace Plugin::Internal::Core::TypeSerializer
{
	inline constexpr auto FORM_DATA_FORMAT =
		"@Form:'{:s}'|'{:s}'|0x{:08X}"_cf;
	inline constexpr auto ALIAS_DATA_FORMAT =
		"@Alias:'{:s}'|{:d}|'{:s}'|0x{:08X}"_cf;
	inline constexpr auto INVENTORY_REF_DATA_FORMAT =
		"@InventoryRef:'{:s}'|{:d}|'{:s}'|0x{:08X}"_cf;
	inline constexpr auto ACTIVE_EFFECT_DATA_FORMAT =
		"@ActiveEffect:'{:s}'|{:d}|'{:s}'|0x{:08X}"_cf;
	inline constexpr auto INPUT_LAYER_DATA_FORMAT =
		"@InputLayer:'{:s}'|{:d}"_cf;

	inline constexpr auto FORM_DATA_REGEX =
		R"(^@Form:'(?<scriptName>.+?)'\|'(?<pluginName>.*?)'\|(?<formId>0[xX][[:xdigit:]]{1,8}+)$)"_ctre;
	inline constexpr auto ALIAS_DATA_REGEX =
		R"(^@Alias:'(?<scriptName>.+?)'\|(?<aliasId>[[:digit:]]{1,10}+)\|'(?<pluginName>.*?)'\|(?<questId>0[xX][[:xdigit:]]{1,8}+)$)"_ctre;
	inline constexpr auto INVENTORY_REF_DATA_REGEX =
		R"(^@InventoryRef:'(?<scriptName>.+?)'\|(?<itemId>[[:digit:]]{1,10}+)\|'(?<pluginName>.*?)'\|(?<containerRefId>0[xX][[:xdigit:]]{1,8}+)$)"_ctre;
	inline constexpr auto ACTIVE_EFFECT_DATA_REGEX =
		R"(^@ActiveEffect:'(?<scriptName>.+?)'\|(?<effectId>[[:digit:]]{1,10}+)\|'(?<pluginName>.*?)'\|(?<actorId>0[xX][[:xdigit:]]{1,8}+)$)"_ctre;
	inline constexpr auto INPUT_LAYER_DATA_REGEX =
		R"(^@InputLayer:'(?<scriptName>.+?)'\|(?<layerId>[[:digit:]]{1,10}+)$)"_ctre;

	static_assert(REX::ToString(std::numeric_limits<std::uint32_t>::max(), REX::IntFormat::kDecimal)->size() == 10);
	static_assert(REX::ToString(std::numeric_limits<RE::TESFormID>::max(), REX::IntFormat::kHexadecimal)->size() == 8);

	constexpr auto SerializeFormData(const FormData& a_value) -> std::string
	{
		return fmt::format(
			FORM_DATA_FORMAT,
			a_value.scriptName,
			a_value.pluginName,
			a_value.formId);
	}

	constexpr auto DeserializeFormData(std::string_view a_value) -> std::optional<FormData>
	{
		// NOLINTNEXTLINE(readability-static-accessed-through-instance)
		const auto match = FORM_DATA_REGEX.match(a_value);
		if (!match) {
			return std::nullopt;
		}

		const auto scriptName = match.get<"scriptName">().to_view();
		const auto pluginName = match.get<"pluginName">().to_view();
		const auto formIdString = match.get<"formId">().to_view();

		const auto formId = REX::FromString<RE::TESFormID>(formIdString, REX::IntFormat::kHexadecimal);
		if (!formId.has_value() || formId.value() == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		return FormData{
			.scriptName = scriptName,
			.pluginName = pluginName,
			.formId = formId.value()
		};
	}

	constexpr auto SerializeAliasData(const AliasData& a_value) -> std::string
	{
		return fmt::format(
			ALIAS_DATA_FORMAT,
			a_value.scriptName,
			a_value.aliasId,
			a_value.pluginName,
			a_value.questId);
	}

	constexpr auto DeserializeAliasData(std::string_view a_value) -> std::optional<AliasData>
	{
		// NOLINTNEXTLINE(readability-static-accessed-through-instance)
		const auto match = ALIAS_DATA_REGEX.match(a_value);
		if (!match) {
			return std::nullopt;
		}

		const auto scriptName = match.get<"scriptName">().to_view();
		const auto aliasIdString = match.get<"aliasId">().to_view();
		const auto pluginName = match.get<"pluginName">().to_view();
		const auto questIdString = match.get<"questId">().to_view();

		const auto aliasId = REX::FromString<std::uint32_t>(aliasIdString, REX::IntFormat::kDecimal);
		if (!aliasId.has_value()) {
			return std::nullopt;
		}

		const auto questId = REX::FromString<RE::TESFormID>(questIdString, REX::IntFormat::kHexadecimal);
		if (!questId.has_value() || questId.value() == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		return AliasData{
			.scriptName = scriptName,
			.aliasId = aliasId.value(),
			.pluginName = pluginName,
			.questId = questId.value()
		};
	}

	constexpr auto SerializeInventoryRefData(const InventoryRefData& a_value) -> std::string
	{
		return fmt::format(
			INVENTORY_REF_DATA_FORMAT,
			a_value.scriptName,
			a_value.itemId,
			a_value.pluginName,
			a_value.containerRefId);
	}

	constexpr auto DeserializeInventoryRefData(std::string_view a_value) -> std::optional<InventoryRefData>
	{
		// NOLINTNEXTLINE(readability-static-accessed-through-instance)
		const auto match = INVENTORY_REF_DATA_REGEX.match(a_value);
		if (!match) {
			return std::nullopt;
		}

		const auto scriptName = match.get<"scriptName">().to_view();
		const auto itemIdString = match.get<"itemId">().to_view();
		const auto pluginName = match.get<"pluginName">().to_view();
		const auto containerRefIdString = match.get<"containerRefId">().to_view();

		const auto itemId = REX::FromString<std::uint32_t>(itemIdString, REX::IntFormat::kDecimal);
		if (!itemId.has_value()) {
			return std::nullopt;
		}

		const auto containerRefId = REX::FromString<RE::TESFormID>(containerRefIdString, REX::IntFormat::kHexadecimal);
		if (!containerRefId.has_value() || containerRefId.value() == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		return InventoryRefData{
			.scriptName = scriptName,
			.itemId = itemId.value(),
			.pluginName = pluginName,
			.containerRefId = containerRefId.value()
		};
	}

	constexpr auto SerializeActiveEffectData(const ActiveEffectData& a_value) -> std::string
	{
		return fmt::format(
			ACTIVE_EFFECT_DATA_FORMAT,
			a_value.scriptName,
			a_value.effectId,
			a_value.pluginName,
			a_value.actorId);
	}

	constexpr auto DeserializeActiveEffectData(std::string_view a_value) -> std::optional<ActiveEffectData>
	{
		// NOLINTNEXTLINE(readability-static-accessed-through-instance)
		const auto match = ACTIVE_EFFECT_DATA_REGEX.match(a_value);
		if (!match) {
			return std::nullopt;
		}

		const auto scriptName = match.get<"scriptName">().to_view();
		const auto effectIdString = match.get<"effectId">().to_view();
		const auto pluginName = match.get<"pluginName">().to_view();
		const auto actorIdString = match.get<"actorId">().to_view();

		const auto effectId = REX::FromString<std::uint32_t>(effectIdString, REX::IntFormat::kDecimal);
		if (!effectId.has_value()) {
			return std::nullopt;
		}

		const auto actorId = REX::FromString<RE::TESFormID>(actorIdString, REX::IntFormat::kHexadecimal);
		if (!actorId.has_value() || actorId.value() == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		return ActiveEffectData{
			.scriptName = scriptName,
			.effectId = effectId.value(),
			.pluginName = pluginName,
			.actorId = actorId.value()
		};
	}

	constexpr auto SerializeInputLayerData(const InputLayerData& a_value) -> std::string
	{
		return fmt::format(
			INPUT_LAYER_DATA_FORMAT,
			a_value.scriptName,
			a_value.layerId);
	}

	constexpr auto DeserializeInputLayerData(std::string_view a_value) -> std::optional<InputLayerData>
	{
		// NOLINTNEXTLINE(readability-static-accessed-through-instance)
		const auto match = INPUT_LAYER_DATA_REGEX.match(a_value);
		if (!match) {
			return std::nullopt;
		}

		const auto scriptName = match.get<"scriptName">().to_view();
		const auto layerIdString = match.get<"layerId">().to_view();

		const auto layerId = REX::FromString<std::uint32_t>(layerIdString, REX::IntFormat::kDecimal);
		if (!layerId.has_value()) {
			return std::nullopt;
		}

		return InputLayerData{
			.scriptName = scriptName,
			.layerId = layerId.value()
		};
	}
}
