#include "Plugin/Scaleform/Form.hpp"

#include "Plugin/Internal/Core/ScaleformUtility.hpp"

namespace Plugin::Scaleform::Form
{
	class getPlayerRef final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getPlayerRef() = default;
		~getPlayerRef() override = default;

		getPlayerRef(const getPlayerRef&) = delete;
		getPlayerRef(getPlayerRef&&) = delete;

		getPlayerRef& operator=(const getPlayerRef&) = delete;
		getPlayerRef& operator=(getPlayerRef&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 0) {
				paramResult->SetNull();
				return;
			}

			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			auto gfxPlayer = ::Plugin::Internal::Core::ScaleformUtility::FromForm(player, a_params.GetMovieRoot());
			if (!gfxPlayer.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxPlayer).value());
		}
	};

	class getFormById final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getFormById() = default;
		~getFormById() override = default;

		getFormById(const getFormById&) = delete;
		getFormById(getFormById&&) = delete;

		getFormById& operator=(const getFormById&) = delete;
		getFormById& operator=(getFormById&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNull();
				return;
			}

			const auto formId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!formId.has_value()) {
				paramResult->SetNull();
				return;
			}

			auto* form = RE::TESForm::FindFormByNumericID(formId.value());
			if (!form) {
				paramResult->SetNull();
				return;
			}

			auto gfxForm = ::Plugin::Internal::Core::ScaleformUtility::FromForm(form, a_params.GetMovieRoot());
			if (!gfxForm.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxForm).value());
		}
	};

	class getFormByEditorId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getFormByEditorId() = default;
		~getFormByEditorId() override = default;

		getFormByEditorId(const getFormByEditorId&) = delete;
		getFormByEditorId(getFormByEditorId&&) = delete;

		getFormByEditorId& operator=(const getFormByEditorId&) = delete;
		getFormByEditorId& operator=(getFormByEditorId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNull();
				return;
			}

			const auto editorId = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!editorId.has_value()) {
				paramResult->SetNull();
				return;
			}

			auto* form = RE::TESForm::FindFormByEditorID(editorId.value());
			if (!form) {
				paramResult->SetNull();
				return;
			}

			auto gfxForm = ::Plugin::Internal::Core::ScaleformUtility::FromForm(form, a_params.GetMovieRoot());
			if (!gfxForm.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxForm).value());
		}
	};

	class getFormFromPlugin final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getFormFromPlugin() = default;
		~getFormFromPlugin() override = default;

		getFormFromPlugin(const getFormFromPlugin&) = delete;
		getFormFromPlugin(getFormFromPlugin&&) = delete;

		getFormFromPlugin& operator=(const getFormFromPlugin&) = delete;
		getFormFromPlugin& operator=(getFormFromPlugin&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			const auto localFormId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!localFormId.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto pluginName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!pluginName.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			auto* form = dataHandler->FindForm(localFormId.value(), pluginName.value());
			if (!form) {
				paramResult->SetNull();
				return;
			}

			auto gfxForm = ::Plugin::Internal::Core::ScaleformUtility::FromForm(form, a_params.GetMovieRoot());
			if (!gfxForm.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxForm).value());
		}
	};

	class getAliasById final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getAliasById() = default;
		~getAliasById() override = default;

		getAliasById(const getAliasById&) = delete;
		getAliasById(getAliasById&&) = delete;

		getAliasById& operator=(const getAliasById&) = delete;
		getAliasById& operator=(getAliasById&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			const auto aliasId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!aliasId.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[1]);
			if (!formOrRef.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetNull();
				return;
			}

			const auto* quest = RE::DynamicCast<const RE::TESQuest*>(form);
			if (!quest) {
				paramResult->SetNull();
				return;
			}

			const auto* alias = quest->GetAliasByID(aliasId.value());
			if (!alias) {
				paramResult->SetNull();
				return;
			}

			auto gfxAlias = ::Plugin::Internal::Core::ScaleformUtility::FromAlias(alias, a_params.GetMovieRoot());
			if (!gfxAlias.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxAlias).value());
		}
	};

	class getInventoryRefById final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getInventoryRefById() = default;
		~getInventoryRefById() override = default;

		getInventoryRefById(const getInventoryRefById&) = delete;
		getInventoryRefById(getInventoryRefById&&) = delete;

		getInventoryRefById& operator=(const getInventoryRefById&) = delete;
		getInventoryRefById& operator=(getInventoryRefById&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			const auto itemId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!itemId.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[1]);
			if (!formOrRef.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto& containerRef = formOrRef->GetRef();
			if (!containerRef) {
				paramResult->SetNull();
				return;
			}

			const auto itemRef = RE::GameScript::GetRefInventoryObjectRef(containerRef, static_cast<std::uint16_t>(itemId.value()));
			if (!itemRef) {
				paramResult->SetNull();
				return;
			}

			const auto inventoryItemRef = ::Plugin::Internal::Core::TypeSerializer::InventoryItemRef(
				containerRef, itemId.value());

			auto gfxItemRef = ::Plugin::Internal::Core::ScaleformUtility::FromInventoryRef(inventoryItemRef, a_params.GetMovieRoot());
			if (!gfxItemRef.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxItemRef).value());
		}
	};

	class getActiveEffectById final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getActiveEffectById() = default;
		~getActiveEffectById() override = default;

		getActiveEffectById(const getActiveEffectById&) = delete;
		getActiveEffectById(getActiveEffectById&&) = delete;

		getActiveEffectById& operator=(const getActiveEffectById&) = delete;
		getActiveEffectById& operator=(getActiveEffectById&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			const auto activeEffectId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!activeEffectId.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[1]);
			if (!formOrRef.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetNull();
				return;
			}

			const auto* actor = RE::DynamicCast<const RE::Actor*>(form);
			if (!actor) {
				paramResult->SetNull();
				return;
			}

			const auto activeEffect = actor->GetActiveEffectByID(static_cast<std::uint16_t>(activeEffectId.value()));
			if (!activeEffect) {
				paramResult->SetNull();
				return;
			}

			auto gfxActiveEffect = ::Plugin::Internal::Core::ScaleformUtility::FromActiveEffect(activeEffect, a_params.GetMovieRoot());
			if (!gfxActiveEffect.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxActiveEffect).value());
		}
	};

	class getInputLayerById final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getInputLayerById() = default;
		~getInputLayerById() override = default;

		getInputLayerById(const getInputLayerById&) = delete;
		getInputLayerById(getInputLayerById&&) = delete;

		getInputLayerById& operator=(const getInputLayerById&) = delete;
		getInputLayerById& operator=(getInputLayerById&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNull();
				return;
			}

			const auto layerId = ::Plugin::Internal::Core::ScaleformUtility::ToUInt(a_params.args[0]);
			if (!layerId.has_value()) {
				paramResult->SetNull();
				return;
			}

			auto* inputLayerManager = RE::BSInputEnableManager::GetSingleton();
			if (!inputLayerManager) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			const auto inputLayer = inputLayerManager->GetLayerByID(layerId.value());
			if (!inputLayer) {
				paramResult->SetNull();
				return;
			}

			auto gfxInputLayer = ::Plugin::Internal::Core::ScaleformUtility::FromInputLayer(inputLayer, a_params.GetMovieRoot());
			if (!gfxInputLayer.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxInputLayer).value());
		}
	};

	class getFormId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getFormId() = default;
		~getFormId() override = default;

		getFormId(const getFormId&) = delete;
		getFormId(getFormId&&) = delete;

		getFormId& operator=(const getFormId&) = delete;
		getFormId& operator=(getFormId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetUInt(0);
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[0]);
			if (!formOrRef.has_value()) {
				paramResult->SetUInt(0);
				return;
			}

			const auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetUInt(0);
				return;
			}

			paramResult->SetUInt(form->GetFormID());
		}
	};

	class getFormEditorId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getFormEditorId() = default;
		~getFormEditorId() override = default;

		getFormEditorId(const getFormEditorId&) = delete;
		getFormEditorId(getFormEditorId&&) = delete;

		getFormEditorId& operator=(const getFormEditorId&) = delete;
		getFormEditorId& operator=(getFormEditorId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNull();
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[0]);
			if (!formOrRef.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetNull();
				return;
			}

			const auto editorId = RE::TESForm::GetEditorIDByForm(form);
			if (!editorId.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto movieRoot = a_params.GetMovieRoot();
			if (!movieRoot) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			auto gfxEditorId = ::Scaleform::GFx::Value();
			movieRoot->CreateString(std::addressof(gfxEditorId), editorId->data());

			paramResult->operator=(std::move(gfxEditorId));
		}
	};

	class getAliasId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getAliasId() = default;
		~getAliasId() override = default;

		getAliasId(const getAliasId&) = delete;
		getAliasId(getAliasId&&) = delete;

		getAliasId& operator=(const getAliasId&) = delete;
		getAliasId& operator=(getAliasId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetUInt(0);
				return;
			}

			const auto alias = ::Plugin::Internal::Core::ScaleformUtility::ToAlias(a_params.args[0]);
			if (!alias.has_value() || !alias.value()) {
				paramResult->SetUInt(0);
				return;
			}

			paramResult->SetUInt((*alias)->aliasID);
		}
	};

	class getInventoryRefId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getInventoryRefId() = default;
		~getInventoryRefId() override = default;

		getInventoryRefId(const getInventoryRefId&) = delete;
		getInventoryRefId(getInventoryRefId&&) = delete;

		getInventoryRefId& operator=(const getInventoryRefId&) = delete;
		getInventoryRefId& operator=(getInventoryRefId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetUInt(0);
				return;
			}

			const auto inventoryRef = ::Plugin::Internal::Core::ScaleformUtility::ToInventoryRef(a_params.args[0]);
			if (!inventoryRef.has_value()) {
				paramResult->SetUInt(0);
				return;
			}

			paramResult->SetUInt(inventoryRef->GetItemId());
		}
	};

	class getActiveEffectId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getActiveEffectId() = default;
		~getActiveEffectId() override = default;

		getActiveEffectId(const getActiveEffectId&) = delete;
		getActiveEffectId(getActiveEffectId&&) = delete;

		getActiveEffectId& operator=(const getActiveEffectId&) = delete;
		getActiveEffectId& operator=(getActiveEffectId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetUInt(0);
				return;
			}

			const auto activeEffect = ::Plugin::Internal::Core::ScaleformUtility::ToActiveEffect(a_params.args[0]);
			if (!activeEffect.has_value() || !activeEffect.value()) {
				paramResult->SetUInt(0);
				return;
			}

			paramResult->SetUInt((*activeEffect)->uniqueID);
		}
	};

	class getInputLayerId final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getInputLayerId() = default;
		~getInputLayerId() override = default;

		getInputLayerId(const getInputLayerId&) = delete;
		getInputLayerId(getInputLayerId&&) = delete;

		getInputLayerId& operator=(const getInputLayerId&) = delete;
		getInputLayerId& operator=(getInputLayerId&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetUInt(0);
				return;
			}

			const auto inputLayer = ::Plugin::Internal::Core::ScaleformUtility::ToInputLayer(a_params.args[0]);
			if (!inputLayer.has_value() || !inputLayer.value()) {
				paramResult->SetUInt(0);
				return;
			}

			paramResult->SetUInt((*inputLayer)->layerID);
		}
	};

	class getGlobalValue final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getGlobalValue() = default;
		~getGlobalValue() override = default;

		getGlobalValue(const getGlobalValue&) = delete;
		getGlobalValue(getGlobalValue&&) = delete;

		getGlobalValue& operator=(const getGlobalValue&) = delete;
		getGlobalValue& operator=(getGlobalValue&&) = delete;

		void Call(const Params& a_params) override
		{
			constexpr auto DEFAULT_RESULT = std::numeric_limits<REX::Float64>::quiet_NaN();

			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNumber(DEFAULT_RESULT);
				return;
			}

			const auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[0]);
			if (!formOrRef.has_value()) {
				paramResult->SetNumber(DEFAULT_RESULT);
				return;
			}

			const auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetNumber(DEFAULT_RESULT);
				return;
			}

			const auto* globalForm = RE::DynamicCast<const RE::TESGlobal*>(form);
			if (!globalForm) {
				paramResult->SetNumber(DEFAULT_RESULT);
				return;
			}

			const auto globalValue = globalForm->GetValue();
			paramResult->SetNumber(globalValue);
		}
	};

	class setGlobalValue final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		setGlobalValue() = default;
		~setGlobalValue() override = default;

		setGlobalValue(const setGlobalValue&) = delete;
		setGlobalValue(setGlobalValue&&) = delete;

		setGlobalValue& operator=(const setGlobalValue&) = delete;
		setGlobalValue& operator=(setGlobalValue&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetBoolean(false);
				return;
			}

			auto formOrRef = ::Plugin::Internal::Core::ScaleformUtility::ToForm(a_params.args[0]);
			if (!formOrRef.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto* form = formOrRef->GetEitherAsForm();
			if (!form) {
				paramResult->SetBoolean(false);
				return;
			}

			auto* globalForm = RE::DynamicCast<RE::TESGlobal*>(form);
			if (!globalForm) {
				paramResult->SetBoolean(false);
				return;
			}

			auto globalValue = ::Plugin::Internal::Core::ScaleformUtility::ToFloat(a_params.args[1]);
			if (!globalValue.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			globalForm->SetValue(globalValue.value());
			paramResult->SetBoolean(true);
		}
	};

	void RegisterFunctions(::Scaleform::GFx::Movie* a_movie, ::Scaleform::GFx::Value* a_value)
	{
		if (!a_movie) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		if (!a_value) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getPlayerRef);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getFormById);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getFormByEditorId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getFormFromPlugin);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getAliasById);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getInventoryRefById);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getActiveEffectById);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getInputLayerById);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getFormId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getFormEditorId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getAliasId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getInventoryRefId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getActiveEffectId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getInputLayerId);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getGlobalValue);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, setGlobalValue);
	}
}
