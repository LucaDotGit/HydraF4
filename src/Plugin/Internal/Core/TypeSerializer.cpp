#include "Plugin/Internal/Core/TypeSerializer.hpp"

namespace Plugin::Internal::Core::TypeSerializer::Impl
{
	static auto GetFormVariantFormType(const FormVariant::value_type& a_value) -> std::optional<RE::FormType>
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(a_value));
		if (formPtr) {
			const auto* form = *formPtr;
			if (!form) {
				return std::nullopt;
			}

			return form->GetFormType();
		}

		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(a_value));
		if (refPtr) {
			const auto ref = *refPtr;
			if (!ref) {
				return std::nullopt;
			}

			return ref->GetFormType();
		}

		std::unreachable();
	}
}

namespace Plugin::Internal::Core::TypeSerializer
{
	FormVariant::FormVariant(std::nullptr_t)
		: _value(static_cast<REX::Observer<RE::TESForm*>>(nullptr))
	{
	}

	FormVariant::FormVariant(REX::Observer<RE::TESForm*> a_form)
		: _value(a_form)
	{
	}

	FormVariant::FormVariant(REX::Observer<RE::TESObjectREFR*> a_ref)
		: _value(a_ref)
	{
	}

	FormVariant::FormVariant(RE::NiPointer<RE::TESObjectREFR> a_ref)
		: _value(std::move(a_ref))
	{
	}

	FormVariant::FormVariant(RE::ObjectRefHandle a_refHandle)
		: _value(a_refHandle.get())
	{
	}

	FormVariant::FormVariant(value_type&& a_value)
		: _value(std::move(a_value))
	{
	}

	FormVariant& FormVariant::operator=(std::nullptr_t)
	{
		_value = static_cast<REX::Observer<RE::TESForm*>>(nullptr);
		return *this;
	}

	FormVariant& FormVariant::operator=(REX::Observer<RE::TESForm*> a_form)
	{
		_value = a_form;
		return *this;
	}

	FormVariant& FormVariant::operator=(REX::Observer<RE::TESObjectREFR*> a_ref)
	{
		_value = a_ref;
		return *this;
	}

	FormVariant& FormVariant::operator=(RE::NiPointer<RE::TESObjectREFR> a_ref)
	{
		_value = std::move(a_ref);
		return *this;
	}

	FormVariant& FormVariant::operator=(RE::ObjectRefHandle a_refHandle)
	{
		_value = a_refHandle.get();
		return *this;
	}

	FormVariant& FormVariant::operator=(value_type&& a_value)
	{
		_value = std::move(a_value);
		return *this;
	}

	FormVariant::operator bool() const noexcept
	{
		return IsValid();
	}

	auto FormVariant::IsValid() const noexcept -> bool
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(_value));
		if (formPtr) {
			return (*formPtr) != nullptr;
		}

		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(_value));
		if (refPtr) {
			return (*refPtr) != nullptr;
		}

		std::unreachable();
	}

	auto FormVariant::IsForm() const noexcept -> bool
	{
		return std::holds_alternative<REX::Observer<RE::TESForm*>>(_value);
	}

	auto FormVariant::IsRef() const noexcept -> bool
	{
		return std::holds_alternative<RE::NiPointer<RE::TESObjectREFR>>(_value);
	}

	auto FormVariant::GetForm() noexcept -> REX::Observer<RE::TESForm*>
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(_value));
		if (!formPtr) {
			return nullptr;
		}

		return *formPtr;
	}

	auto FormVariant::GetForm() const noexcept -> REX::Observer<const RE::TESForm*>
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(_value));
		if (!formPtr) {
			return nullptr;
		}

		return *formPtr;
	}

	auto FormVariant::GetRef() const noexcept -> RE::NiPointer<RE::TESObjectREFR>
	{
		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(_value));
		if (!refPtr) {
			return nullptr;
		}

		return *refPtr;
	}

	auto FormVariant::GetRefHandle() const noexcept -> RE::ObjectRefHandle
	{
		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(_value));
		if (!refPtr) {
			return {};
		}

		const auto& ref = *refPtr;
		if (!ref) {
			return {};
		}

		return ref->GetHandle();
	}

	auto FormVariant::GetEitherAsForm() noexcept -> REX::Observer<RE::TESForm*>
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(_value));
		if (formPtr) {
			return *formPtr;
		}

		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(_value));
		if (refPtr) {
			return refPtr->get();
		}

		std::unreachable();
	}

	auto FormVariant::GetEitherAsForm() const noexcept -> REX::Observer<const RE::TESForm*>
	{
		const auto* formPtr = std::get_if<REX::Observer<RE::TESForm*>>(std::addressof(_value));
		if (formPtr) {
			return *formPtr;
		}

		const auto* refPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(_value));
		if (refPtr) {
			return refPtr->get();
		}

		std::unreachable();
	}

	InventoryItemRef::InventoryItemRef(std::nullptr_t)
	{
	}

	InventoryItemRef::InventoryItemRef(REX::Observer<RE::TESObjectREFR*> a_ref, std::uint32_t a_itemId)
		: _containerRef(a_ref),
		  _itemId(a_itemId)
	{
	}

	InventoryItemRef::InventoryItemRef(RE::ObjectRefHandle a_refHandle, std::uint32_t a_itemId)
		: _containerRef(a_refHandle.get()),
		  _itemId(a_itemId)
	{
	}

	InventoryItemRef::InventoryItemRef(RE::NiPointer<RE::TESObjectREFR> a_ref, std::uint32_t a_itemId)
		: _containerRef(std::move(a_ref)),
		  _itemId(a_itemId)
	{
	}

	InventoryItemRef& InventoryItemRef::operator=(std::nullptr_t)
	{
		_containerRef.reset();
		_itemId = 0;
		return *this;
	}

	InventoryItemRef::operator bool() const noexcept
	{
		return IsValid();
	}

	auto InventoryItemRef::IsValid() const noexcept -> bool
	{
		return _containerRef != nullptr && _itemId != RE::GameScript::RefrOrInventoryObj::INVALID_UNIQUE_ID;
	}

	auto InventoryItemRef::GetContainerRef() const noexcept -> RE::NiPointer<RE::TESObjectREFR>
	{
		return _containerRef;
	}

	auto InventoryItemRef::GetContainerRefHandle() const noexcept -> RE::ObjectRefHandle
	{
		if (!_containerRef) {
			return {};
		}

		return _containerRef->GetHandle();
	}

	auto InventoryItemRef::GetItemId() const noexcept -> std::uint32_t
	{
		return _itemId;
	}

	auto DeserializeObjectData(std::string_view a_value) -> std::optional<ObjectData>
	{
		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		auto formData = DeserializeFormData(a_value);
		if (formData.has_value()) {
			const auto formId = dataHandler->FindFormID(formData->formId, formData->pluginName);
			if (!formId.has_value()) {
				return std::nullopt;
			}

			const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForForm(formId.value());

			return ObjectData{
				.scriptName = formData->scriptName,
				.objectHandle = objectHandle
			};
		}

		auto aliasData = DeserializeAliasData(a_value);
		if (aliasData.has_value()) {
			const auto formId = dataHandler->FindFormID(aliasData->questId, aliasData->pluginName);
			if (!formId.has_value()) {
				return std::nullopt;
			}

			const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForAlias(
				static_cast<std::uint16_t>(aliasData->aliasId), formId.value());

			return ObjectData{
				.scriptName = aliasData->scriptName,
				.objectHandle = objectHandle
			};
		}

		auto inventoryRefData = DeserializeInventoryRefData(a_value);
		if (inventoryRefData.has_value()) {
			const auto containerRefId = dataHandler->FindFormID(inventoryRefData->containerRefId, inventoryRefData->pluginName);
			if (!containerRefId.has_value()) {
				return std::nullopt;
			}

			const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForInventoryObject(
				static_cast<std::uint16_t>(inventoryRefData->itemId), containerRefId.value());

			return ObjectData{
				.scriptName = inventoryRefData->scriptName,
				.objectHandle = objectHandle
			};
		}

		auto activeEffectData = DeserializeActiveEffectData(a_value);
		if (activeEffectData.has_value()) {
			const auto actorId = dataHandler->FindFormID(activeEffectData->actorId, activeEffectData->pluginName);
			if (!actorId.has_value()) {
				return std::nullopt;
			}

			const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForActiveEffect(
				static_cast<std::uint16_t>(activeEffectData->effectId), actorId.value());

			return ObjectData{
				.scriptName = activeEffectData->scriptName,
				.objectHandle = objectHandle
			};
		}

		auto inputLayerData = DeserializeInputLayerData(a_value);
		if (inputLayerData.has_value()) {
			const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForInputEnableLayer(inputLayerData->layerId);

			return ObjectData{
				.scriptName = inputLayerData->scriptName,
				.objectHandle = objectHandle
			};
		}

		return std::nullopt;
	}

	auto SerializeForm(const FormVariant& a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		const auto* formOrRef = a_value.GetEitherAsForm();
		if (!formOrRef) {
			return std::nullopt;
		}

		const auto* plugin = formOrRef->GetFile(0);

		auto formData = FormData();

		formData.pluginName = plugin ? plugin->filename.data() : ""sv;
		if (plugin && formData.pluginName.empty()) {
			return std::nullopt;
		}

		formData.formId = formOrRef->GetFormID();
		if (formData.formId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptName = RE::BSFixedString();
		if (!scriptVM->GetScriptName(formOrRef->GetFormType(), scriptName)) {
			return std::nullopt;
		}

		formData.scriptName = scriptName;
		return SerializeFormData(formData);
	}

	auto DeserializeForm(std::string_view a_value) -> std::optional<FormVariant>
	{
		auto formData = DeserializeFormData(a_value);
		if (!formData.has_value()) {
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		auto formOrRef = dataHandler->FindFormOrRef(formData->formId, formData->pluginName);
		if (!formOrRef.has_value()) {
			return FormVariant();
		}

		const auto formType = Impl::GetFormVariantFormType(formOrRef.value());
		if (!formType.has_value()) {
			return FormVariant();
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return FormVariant();
		}

		if (!scriptVM->IsInstanceOf(formType.value(), RE::BSFixedString(formData->scriptName))) {
			return FormVariant();
		}

		return std::move(formOrRef).value();
	}

	auto SerializeAlias(const RE::BGSBaseAlias* a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		const auto* owningQuest = a_value->GetOwningQuest();
		if (!owningQuest) {
			return std::nullopt;
		}

		const auto* plugin = owningQuest->GetFile(0);
		if (!plugin) {
			return std::nullopt;
		}

		auto aliasData = AliasData();

		aliasData.pluginName = plugin->filename.data();
		if (aliasData.pluginName.empty()) {
			return std::nullopt;
		}

		aliasData.questId = owningQuest->GetFormID();
		if (aliasData.questId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptName = RE::BSFixedString();
		if (!scriptVM->GetScriptName(a_value->GetFormType(), scriptName)) {
			return std::nullopt;
		}

		aliasData.scriptName = scriptName;
		aliasData.aliasId = a_value->GetAliasID();

		return SerializeAliasData(aliasData);
	}

	auto DeserializeAlias(std::string_view a_value) -> std::optional<RE::BGSBaseAlias*>
	{
		auto aliasData = DeserializeAliasData(a_value);
		if (!aliasData.has_value()) {
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		auto* form = dataHandler->FindForm<RE::TESQuest>(aliasData->questId, aliasData->pluginName);
		if (!form) {
			return static_cast<RE::BGSBaseAlias*>(nullptr);
		}

		auto* alias = form->GetAliasByID(aliasData->aliasId);
		if (!alias) {
			return static_cast<RE::BGSBaseAlias*>(nullptr);
		};

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return static_cast<RE::BGSBaseAlias*>(nullptr);
		}

		if (!scriptVM->IsInstanceOf(alias->GetFormType(), RE::BSFixedString(aliasData->scriptName))) {
			return static_cast<RE::BGSBaseAlias*>(nullptr);
		}

		return alias;
	}

	auto SerializeInventoryRef(const InventoryItemRef& a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		const auto containerRef = a_value.GetContainerRef();
		if (!containerRef) {
			return std::nullopt;
		}

		const auto* plugin = containerRef->GetFile(0);
		if (!plugin) {
			return std::nullopt;
		}

		auto inventoryRefData = InventoryRefData();

		inventoryRefData.pluginName = plugin->filename.data();
		if (inventoryRefData.pluginName.empty()) {
			return std::nullopt;
		}

		inventoryRefData.containerRefId = containerRef->GetFormID();
		if (inventoryRefData.containerRefId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptName = RE::BSFixedString();
		if (!scriptVM->GetScriptName(containerRef->GetFormType(), scriptName)) {
			return std::nullopt;
		}

		inventoryRefData.scriptName = scriptName;
		inventoryRefData.itemId = a_value.GetItemId();

		return SerializeInventoryRefData(inventoryRefData);
	}

	auto DeserializeInventoryRef(std::string_view a_value) -> std::optional<InventoryItemRef>
	{
		auto inventoryRefData = DeserializeInventoryRefData(a_value);
		if (!inventoryRefData.has_value()) {
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		const auto formOrRef = dataHandler->FindFormOrRef(inventoryRefData->containerRefId, inventoryRefData->pluginName);
		if (!formOrRef.has_value()) {
			return InventoryItemRef();
		}

		const auto* itemRefPtr = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(formOrRef.value()));
		if (!itemRefPtr) {
			return InventoryItemRef();
		}

		const auto formType = Impl::GetFormVariantFormType(formOrRef.value());
		if (!formType.has_value()) {
			return InventoryItemRef();
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return InventoryItemRef();
		}

		if (!scriptVM->IsInstanceOf(formType.value(), RE::BSFixedString(inventoryRefData->scriptName))) {
			return InventoryItemRef();
		}

		return InventoryItemRef(*itemRefPtr, inventoryRefData->itemId);
	}

	auto SerializeActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		const auto targetActor = a_value->GetTargetActor();
		if (!targetActor) {
			return std::nullopt;
		}

		const auto* plugin = targetActor->GetFile(0);
		if (!plugin) {
			return std::nullopt;
		}

		auto activeEffectData = ActiveEffectData();

		activeEffectData.pluginName = plugin->filename.data();
		if (activeEffectData.pluginName.empty()) {
			return std::nullopt;
		}

		activeEffectData.actorId = targetActor->GetFormID();
		if (activeEffectData.actorId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptName = RE::BSFixedString();
		if (!scriptVM->GetScriptName(a_value->GetFormType(), scriptName)) {
			return std::nullopt;
		}

		activeEffectData.scriptName = scriptName;
		activeEffectData.effectId = a_value->GetUniqueID();

		return SerializeActiveEffectData(activeEffectData);
	}

	auto DeserializeActiveEffect(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>
	{
		auto activeEffectData = DeserializeActiveEffectData(a_value);
		if (!activeEffectData.has_value()) {
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		const auto formOrRef = dataHandler->FindFormOrRef(activeEffectData->actorId, activeEffectData->pluginName);
		if (!formOrRef.has_value()) {
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		const auto* actorRef = std::get_if<RE::NiPointer<RE::TESObjectREFR>>(std::addressof(formOrRef.value()));
		if (!actorRef) {
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		const auto actor = RE::dynamic_ni_pointer_cast<RE::Actor>(*actorRef);
		if (!actor) {
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		auto activeEffect = actor->GetActiveEffectByID(static_cast<std::uint16_t>(activeEffectData->effectId));
		if (!activeEffect) {
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		if (!scriptVM->IsInstanceOf(activeEffect->GetFormType(), RE::BSFixedString(activeEffectData->scriptName))) {
			return RE::BSTSmartPointer<RE::ActiveEffect>();
		}

		return activeEffect;
	}

	auto SerializeInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		auto inputLayerData = InputLayerData();

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptName = RE::BSFixedString();
		if (!scriptVM->GetScriptName(a_value->GetFormType(), scriptName)) {
			return std::nullopt;
		}

		inputLayerData.scriptName = scriptName;
		inputLayerData.layerId = a_value->GetLayerID();

		return SerializeInputLayerData(inputLayerData);
	}

	auto DeserializeInputLayer(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>
	{
		auto inputLayerData = DeserializeInputLayerData(a_value);
		if (!inputLayerData.has_value()) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return RE::BSTSmartPointer<RE::BSInputEnableLayer>();
		}

		const auto* inputLayerManager = RE::BSInputEnableManager::GetSingleton();
		if (!inputLayerManager) [[unlikely]] {
			REX::Fail("Failed to get the game's input layer manager."sv);
		}

		auto inputLayer = inputLayerManager->GetLayerByID(inputLayerData->layerId);
		if (!inputLayer) {
			return RE::BSTSmartPointer<RE::BSInputEnableLayer>();
		}

		if (!scriptVM->IsInstanceOf(inputLayer->GetFormType(), RE::BSFixedString(inputLayerData->scriptName))) {
			return RE::BSTSmartPointer<RE::BSInputEnableLayer>();
		}

		return inputLayer;
	}

	auto SerializeObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value) -> std::optional<std::string>
	{
		if (!a_value) {
			return std::nullopt;
		}

		const auto objectHandle = a_value->GetHandle();
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return std::nullopt;
		}

		auto scriptValue = RE::BSScript::Variable();
		scriptValue.SetObject(a_value);

		if (RE::GameScript::HandlePolicy::IsFormHandle(objectHandle)) {
			auto* form = scriptValue.Unpack<RE::TESForm*>();
			if (!form) {
				return std::nullopt;
			}

			return SerializeForm(form);
		}

		if (RE::GameScript::HandlePolicy::IsAliasHandle(objectHandle)) {
			auto* alias = scriptValue.Unpack<RE::BGSBaseAlias*>();
			if (!alias) {
				return std::nullopt;
			}

			return SerializeAlias(alias);
		}

		if (RE::GameScript::HandlePolicy::IsInventoryObjectHandle(objectHandle)) {
			auto scriptInventoryRef = scriptValue.Unpack<RE::GameScript::RefrOrInventoryObj>();
			if (!scriptInventoryRef) {
				return std::nullopt;
			}

			auto inventoryRef = InventoryItemRef(
				scriptInventoryRef.GetContainerRef(), scriptInventoryRef.GetUniqueID());

			return SerializeInventoryRef(inventoryRef);
		}

		if (RE::GameScript::HandlePolicy::IsActiveEffectHandle(objectHandle)) {
			auto activeEffect = scriptValue.Unpack<RE::BSTSmartPointer<RE::ActiveEffect>>();
			if (!activeEffect) {
				return std::nullopt;
			}

			return SerializeActiveEffect(activeEffect);
		}

		if (RE::GameScript::HandlePolicy::IsInputEnableLayerHandle(objectHandle)) {
			auto inputLayer = scriptValue.Unpack<RE::BSTSmartPointer<RE::BSInputEnableLayer>>();
			if (!inputLayer) {
				return std::nullopt;
			}

			return SerializeInputLayer(inputLayer);
		}

		return std::nullopt;
	}

	auto DeserializeObject(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>
	{
		const auto objectData = DeserializeObjectData(a_value);
		if (!objectData.has_value()) {
			return std::nullopt;
		}

		if (objectData->objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return RE::BSTSmartPointer<RE::BSScript::Object>();
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto zeroedScriptName = std::string(objectData->scriptName);

		auto object = RE::BSTSmartPointer<RE::BSScript::Object>();
		if (!scriptVM->GetOrCreateScriptObjectExact(RE::BSFixedString(zeroedScriptName), objectData->objectHandle, object)) {
			return std::nullopt;
		}

		return object;
	}

	// clang-format off
	static_assert(SerializeFormData(
			FormData
			{
				.scriptName = "MyNamespace:MyFormScript"sv,
				.pluginName = "My Cool Plugin.esp"sv,
				.formId = 0x12345678
			}
		) == "@Form:'MyNamespace:MyFormScript'|'My Cool Plugin.esp'|0x12345678"sv);
	// clang-format on

	// clang-format off
	static_assert(
		DeserializeFormData(
			"@Form:'MyNamespace:MyFormScript'|'My Cool Plugin.esp'|0x12345678"sv) == FormData
			{
				.scriptName = "MyNamespace:MyFormScript"sv,
				.pluginName = "My Cool Plugin.esp"sv,
				.formId = 0x12345678
			}
		);
	// clang-format on

	// clang-format off
	static_assert(
		SerializeAliasData(
			AliasData
			{
				.scriptName = "MyNamespace:MyAliasScript"sv,
				.aliasId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.questId = 0x12345678
			}
		) == "@Alias:'MyNamespace:MyAliasScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv);
	// clang-format on

	// clang-format off
	static_assert(
		DeserializeAliasData(
			"@Alias:'MyNamespace:MyAliasScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv) == AliasData
			{
				.scriptName = "MyNamespace:MyAliasScript"sv,
				.aliasId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.questId = 0x12345678
			}
		);
	// clang-format on

	// clang-format off
	static_assert(
		SerializeInventoryRefData(
			InventoryRefData
			{
				.scriptName = "MyNamespace:MyRefScript"sv,
				.itemId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.containerRefId = 0x12345678
			}
		) == "@InventoryRef:'MyNamespace:MyRefScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv);
	// clang-format on

	// clang-format off
	static_assert(
		DeserializeInventoryRefData(
			"@InventoryRef:'MyNamespace:MyRefScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv) == InventoryRefData
			{
				.scriptName = "MyNamespace:MyRefScript"sv,
				.itemId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.containerRefId = 0x12345678
			}
		);
	// clang-format on

	// clang-format off
	static_assert(
		SerializeActiveEffectData(
			ActiveEffectData
			{
				.scriptName = "MyNamespace:MyActiveEffectScript"sv,
				.effectId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.actorId = 0x12345678
			}
		) == "@ActiveEffect:'MyNamespace:MyActiveEffectScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv);
	// clang-format on

	// clang-format off
	static_assert(
		DeserializeActiveEffectData(
			"@ActiveEffect:'MyNamespace:MyActiveEffectScript'|1234567890|'My Cool Plugin.esp'|0x12345678"sv) == ActiveEffectData
			{
				.scriptName = "MyNamespace:MyActiveEffectScript"sv,
				.effectId = 1234567890,
				.pluginName = "My Cool Plugin.esp"sv,
				.actorId = 0x12345678
			}
		);
	// clang-format on

	// clang-format off
	static_assert(
		SerializeInputLayerData(
			InputLayerData
			{
				.scriptName = "MyNamespace:MyInputLayerScript"sv,
				.layerId = 1234567890
			}
		) == "@InputLayer:'MyNamespace:MyInputLayerScript'|1234567890"sv);
	// clang-format on

	// clang-format off
	static_assert(
		DeserializeInputLayerData(
			"@InputLayer:'MyNamespace:MyInputLayerScript'|1234567890"sv) == InputLayerData
			{
				.scriptName = "MyNamespace:MyInputLayerScript"sv,
				.layerId = 1234567890
			}
		);
	// clang-format on
}
