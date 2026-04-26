#pragma once

namespace Plugin::Internal::Core::TypeSerializer
{
	class FormData final
	{
	public:
		[[nodiscard]] constexpr auto operator==(const FormData&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator!=(const FormData&) const noexcept -> bool = default;

		std::string_view scriptName;
		std::string_view pluginName;
		RE::TESFormID formId;
	};

	class AliasData final
	{
	public:
		[[nodiscard]] constexpr auto operator==(const AliasData&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator!=(const AliasData&) const noexcept -> bool = default;

		std::string_view scriptName;
		std::uint32_t aliasId;
		std::string_view pluginName;
		RE::TESFormID questId;
	};

	class InventoryRefData final
	{
	public:
		[[nodiscard]] constexpr auto operator==(const InventoryRefData&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator!=(const InventoryRefData&) const noexcept -> bool = default;

		std::string_view scriptName;
		std::uint32_t itemId;
		std::string_view pluginName;
		RE::TESFormID containerRefId;
	};

	class ActiveEffectData final
	{
	public:
		[[nodiscard]] constexpr auto operator==(const ActiveEffectData&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator!=(const ActiveEffectData&) const noexcept -> bool = default;

		std::string_view scriptName;
		std::uint32_t effectId;
		std::string_view pluginName;
		RE::TESFormID actorId;
	};

	class InputLayerData final
	{
	public:
		[[nodiscard]] constexpr auto operator==(const InputLayerData&) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator!=(const InputLayerData&) const noexcept -> bool = default;

		std::string_view scriptName;
		std::uint32_t layerId;
	};

	class ObjectData final
	{
	public:
		std::string_view scriptName;
		RE::BSScript::ObjectHandle objectHandle{ RE::GameScript::HandlePolicy::EMPTY_HANDLE };
	};

	class FormVariant final
	{
	public:
		using value_type = std::variant<REX::Observer<RE::TESForm*>, RE::NiPointer<RE::TESObjectREFR>>;

		FormVariant() = default;
		~FormVariant() noexcept = default;

		FormVariant(std::nullptr_t);
		FormVariant(REX::Observer<RE::TESForm*> a_form);
		FormVariant(REX::Observer<RE::TESObjectREFR*> a_ref);
		FormVariant(RE::NiPointer<RE::TESObjectREFR> a_ref);
		FormVariant(RE::ObjectRefHandle a_refHandle);
		FormVariant(value_type&& a_value);

		FormVariant(const FormVariant&) = default;
		FormVariant(FormVariant&&) noexcept = default;

		FormVariant& operator=(std::nullptr_t);
		FormVariant& operator=(REX::Observer<RE::TESForm*> a_form);
		FormVariant& operator=(REX::Observer<RE::TESObjectREFR*> a_ref);
		FormVariant& operator=(RE::NiPointer<RE::TESObjectREFR> a_ref);
		FormVariant& operator=(RE::ObjectRefHandle a_refHandle);
		FormVariant& operator=(value_type&& a_value);

		FormVariant& operator=(const FormVariant&) = default;
		FormVariant& operator=(FormVariant&&) noexcept = default;

		[[nodiscard]] auto operator==(const FormVariant&) const noexcept -> bool = default;
		[[nodiscard]] auto operator!=(const FormVariant&) const noexcept -> bool = default;

		[[nodiscard]] explicit operator bool() const noexcept;

		[[nodiscard]] auto IsValid() const noexcept -> bool;

		[[nodiscard]] auto IsForm() const noexcept -> bool;
		[[nodiscard]] auto IsRef() const noexcept -> bool;

		[[nodiscard]] auto GetForm() noexcept -> REX::Observer<RE::TESForm*>;
		[[nodiscard]] auto GetForm() const noexcept -> REX::Observer<const RE::TESForm*>;

		[[nodiscard]] auto GetRef() const noexcept -> RE::NiPointer<RE::TESObjectREFR>;
		[[nodiscard]] auto GetRefHandle() const noexcept -> RE::ObjectRefHandle;

		[[nodiscard]] auto GetEitherAsForm() noexcept -> REX::Observer<RE::TESForm*>;
		[[nodiscard]] auto GetEitherAsForm() const noexcept -> REX::Observer<const RE::TESForm*>;

	private:
		value_type _value;
	};

	class InventoryItemRef final
	{
	public:
		InventoryItemRef() = default;
		~InventoryItemRef() noexcept = default;

		InventoryItemRef(std::nullptr_t);
		InventoryItemRef(REX::Observer<RE::TESObjectREFR*> a_ref, std::uint32_t a_itemId);
		InventoryItemRef(RE::NiPointer<RE::TESObjectREFR> a_ref, std::uint32_t a_itemId);
		InventoryItemRef(RE::ObjectRefHandle a_refHandle, std::uint32_t a_itemId);

		InventoryItemRef(const InventoryItemRef&) = default;
		InventoryItemRef(InventoryItemRef&&) noexcept = default;

		InventoryItemRef& operator=(std::nullptr_t);

		InventoryItemRef& operator=(const InventoryItemRef&) = default;
		InventoryItemRef& operator=(InventoryItemRef&&) noexcept = default;

		[[nodiscard]] auto operator==(const InventoryItemRef&) const noexcept -> bool = default;
		[[nodiscard]] auto operator!=(const InventoryItemRef&) const noexcept -> bool = default;

		[[nodiscard]] explicit operator bool() const noexcept;

		[[nodiscard]] auto IsValid() const noexcept -> bool;

		[[nodiscard]] auto GetContainerRef() const noexcept -> RE::NiPointer<RE::TESObjectREFR>;
		[[nodiscard]] auto GetContainerRefHandle() const noexcept -> RE::ObjectRefHandle;
		[[nodiscard]] auto GetItemId() const noexcept -> std::uint32_t;

	private:
		RE::NiPointer<RE::TESObjectREFR> _containerRef;
		std::uint32_t _itemId{ 0 };
	};

	[[nodiscard]] constexpr auto SerializeFormData(const FormData& a_value) -> std::string;
	[[nodiscard]] constexpr auto DeserializeFormData(std::string_view a_value) -> std::optional<FormData>;

	[[nodiscard]] constexpr auto SerializeAliasData(const AliasData& a_value) -> std::string;
	[[nodiscard]] constexpr auto DeserializeAliasData(std::string_view a_value) -> std::optional<AliasData>;

	[[nodiscard]] constexpr auto SerializeInventoryRefData(const InventoryRefData& a_value) -> std::string;
	[[nodiscard]] constexpr auto DeserializeInventoryRefData(std::string_view a_value) -> std::optional<InventoryRefData>;

	[[nodiscard]] constexpr auto SerializeActiveEffectData(const ActiveEffectData& a_value) -> std::string;
	[[nodiscard]] constexpr auto DeserializeActiveEffectData(std::string_view a_value) -> std::optional<ActiveEffectData>;

	[[nodiscard]] constexpr auto SerializeInputLayerData(const InputLayerData& a_value) -> std::string;
	[[nodiscard]] constexpr auto DeserializeInputLayerData(std::string_view a_value) -> std::optional<InputLayerData>;

	[[nodiscard]] auto DeserializeObjectData(std::string_view a_value) -> std::optional<ObjectData>;

	[[nodiscard]] auto SerializeForm(const FormVariant& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeForm(std::string_view a_value) -> std::optional<FormVariant>;

	[[nodiscard]] auto SerializeAlias(const RE::BGSBaseAlias* a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeAlias(std::string_view a_value) -> std::optional<RE::BGSBaseAlias*>;

	[[nodiscard]] auto SerializeInventoryRef(const InventoryItemRef& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeInventoryRef(std::string_view a_value) -> std::optional<InventoryItemRef>;

	[[nodiscard]] auto SerializeActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeActiveEffect(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>;

	[[nodiscard]] auto SerializeInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeInputLayer(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>;

	[[nodiscard]] auto SerializeObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value) -> std::optional<std::string>;
	[[nodiscard]] auto DeserializeObject(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>;
}

#include "Plugin/Internal/Core/TypeSerializer.inl"
