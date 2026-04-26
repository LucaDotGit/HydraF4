#pragma once

#include "Plugin/Internal/Core/TypeSerializer.hpp"

namespace Plugin::Internal::Core::ScaleformUtility
{
	[[nodiscard]] auto ToNull(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::nullptr_t>;
	[[nodiscard]] auto ToInt(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::int32_t>;
	[[nodiscard]] auto ToUInt(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::uint32_t>;
	[[nodiscard]] auto ToFloat(const ::Scaleform::GFx::Value& a_value) -> std::optional<REX::Float32>;
	[[nodiscard]] auto ToDouble(const ::Scaleform::GFx::Value& a_value) -> std::optional<REX::Float64>;

	[[nodiscard]] auto FromFormData(const Core::TypeSerializer::FormData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToFormData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::FormData>;

	[[nodiscard]] auto FromAliasData(const Core::TypeSerializer::AliasData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToAliasData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::AliasData>;

	[[nodiscard]] auto FromInventoryRefData(const Core::TypeSerializer::InventoryRefData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToInventoryRefData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InventoryRefData>;

	[[nodiscard]] auto FromActiveEffectData(const Core::TypeSerializer::ActiveEffectData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToActiveEffectData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::ActiveEffectData>;

	[[nodiscard]] auto FromInputLayerData(const Core::TypeSerializer::InputLayerData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToInputLayerData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InputLayerData>;

	[[nodiscard]] auto FromString(std::string_view a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToString(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto ToFixedString(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSFixedString>;

	[[nodiscard]] auto FromForm(const Core::TypeSerializer::FormVariant& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToForm(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::FormVariant>;

	[[nodiscard]] auto FromAlias(const RE::BGSBaseAlias* a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToAlias(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BGSBaseAlias*>;

	[[nodiscard]] auto FromInventoryRef(const Core::TypeSerializer::InventoryItemRef& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToInventoryRef(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InventoryItemRef>;

	[[nodiscard]] auto FromActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToActiveEffect(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>;

	[[nodiscard]] auto FromInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToInputLayer(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>;

	[[nodiscard]] auto FromObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToObject(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>;

	[[nodiscard]] auto FromStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToStruct(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>;

	[[nodiscard]] auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToArray(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>;

	[[nodiscard]] auto FromVar(RE::BSScript::SharedVariant a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToVar(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>;

	[[nodiscard]] auto FromValue(const RE::BSScript::Variable& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>;
	[[nodiscard]] auto ToValue(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>;

	[[nodiscard]] auto ToDetectedValue(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSScript::Variable>;
}
