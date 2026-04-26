#include "Plugin/Internal/Game/GameEditorIds.hpp"

#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::Game
{
	GameEditorIds::GameEditorIds(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _settingsManager(a_settingsManager)
	{
	}

	GameEditorIds::~GameEditorIds() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl GameEditorIds::ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource)
	{
		auto* newForm = RE::TESForm::FindFormByNumericID(a_event.newFormID);
		if (!newForm) {
			return RE::BSEventNotifyControl::kContinue;
		}

		MoveFormEditorId(a_event.oldFormID, newForm);
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl GameEditorIds::ProcessEvent(const RE::TESFormDeleteEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource)
	{
		RemoveFormEditorId(a_event.formID);
		return RE::BSEventNotifyControl::kContinue;
	}

	void GameEditorIds::OnGameDataReady()
	{
		RegisterEvents();
	}

	void GameEditorIds::RegisterEvents()
	{
		auto* formIdRemapEventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (formIdRemapEventSource) {
			formIdRemapEventSource->RegisterSink(this);
		}

		auto* formDeleteEventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (formDeleteEventSource) {
			formDeleteEventSource->RegisterSink(this);
		}
	}

	void GameEditorIds::UnregisterEvents()
	{
		auto* formIdRemapEventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (formIdRemapEventSource) {
			formIdRemapEventSource->UnregisterSink(this);
		}

		auto* formDeleteEventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (formDeleteEventSource) {
			formDeleteEventSource->UnregisterSink(this);
		}
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	auto GameEditorIds::GetFormByEditorId(const RE::BSFixedString& a_editorId) const -> std::optional<std::variant<REX::Observer<RE::TESForm*>, RE::NiPointer<RE::TESObjectREFR>>>
	{
		return RE::TESObjectREFR::FindFormOrRefByEditorID(a_editorId);
	}

	auto GameEditorIds::GetFormEditorId(REX::Observer<const RE::TESForm*> a_form) const -> std::optional<RE::BSFixedString>
	{
		if (!a_form) {
			return std::nullopt;
		}

		const auto&& [_, editorIdMutex] = RE::TESForm::GetAllFormsByEditorID();
		const auto editorIdLock = RE::BSAutoReadLock(editorIdMutex);

		const auto formIdIt = _formIdToEditorIdMap.find(a_form->GetFormID());
		if (formIdIt == _formIdToEditorIdMap.end()) {
			return std::nullopt;
		}

		return formIdIt->second;
	}

	bool GameEditorIds::SetFormEditorId(REX::Observer<RE::TESForm*> a_form, const RE::BSFixedString& a_editorId)
	{
		if (!a_form || a_editorId.empty()) {
			return false;
		}

		const auto newFormId = a_form->GetFormID();
		if (newFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return false;
		}

		const auto&& [editorIdMap, editorIdMutex] = RE::TESForm::GetAllFormsByEditorID();
		if (!editorIdMap) {
			return false;
		}

		const auto editorIdLock = RE::BSAutoWriteLock(editorIdMutex);

		std::call_once(_reserveOnceFlag, [this, &editorIdMap]() {
			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Fail("Failed to get the game's data handler."sv);
			}

			const auto totalFormCount = dataHandler->GetTotalLoadedFormCount();

			_formIdToEditorIdMap.reserve(totalFormCount);
			editorIdMap->reserve(totalFormCount);
		});

		const auto&& [formIdIt, hasFormIdInserted] = _formIdToEditorIdMap.emplace(newFormId, a_editorId);
		if (!hasFormIdInserted) {
			auto& oldEditorId = formIdIt->second;
			if (oldEditorId == a_editorId) {
				return false;
			}

			editorIdMap->erase(oldEditorId);
			oldEditorId = a_editorId;
		}

		const auto&& [editorIdIt, hasEditorIdInserted] = editorIdMap->emplace(a_editorId, a_form);
		if (hasEditorIdInserted) {
			return true;
		}

		const auto* oldForm = editorIdIt->second;
		const auto oldFormId = oldForm ? oldForm->GetFormID() : RE::TESDataHandler::EMPTY_FORM_ID;
		LogDuplicateEditorId(oldFormId, newFormId, a_editorId);

		return true;
	}

	bool GameEditorIds::MoveFormEditorId(RE::TESFormID a_oldFormId, REX::Observer<RE::TESForm*> a_newForm)
	{
		if (a_oldFormId == RE::TESDataHandler::EMPTY_FORM_ID || !a_newForm) {
			return false;
		}

		const auto newFormId = a_newForm->GetFormID();
		if (newFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return false;
		}

		if (newFormId == a_oldFormId) {
			return false;
		}

		const auto&& [editorIdMap, editorIdMutex] = RE::TESForm::GetAllFormsByEditorID();
		if (!editorIdMap) {
			return false;
		}

		const auto editorIdLock = RE::BSAutoWriteLock(editorIdMutex);

		const auto oldEditorIdIt = _formIdToEditorIdMap.find(a_oldFormId);
		if (oldEditorIdIt == _formIdToEditorIdMap.end()) {
			return false;
		}

		const auto oldEditorId = oldEditorIdIt->second;

		_formIdToEditorIdMap.erase(oldEditorIdIt);
		_formIdToEditorIdMap.insert_or_assign(newFormId, oldEditorId);

		editorIdMap->insert_or_assign(oldEditorId, a_newForm);
		return true;
	}

	bool GameEditorIds::RemoveFormEditorId(RE::TESFormID a_formId)
	{
		if (a_formId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return false;
		}

		const auto&& [editorIdMap, editorIdMutex] = RE::TESForm::GetAllFormsByEditorID();
		if (!editorIdMap) {
			return false;
		}

		const auto editorIdLock = RE::BSAutoWriteLock(editorIdMutex);

		const auto formIdIt = _formIdToEditorIdMap.find(a_formId);
		if (formIdIt == _formIdToEditorIdMap.end()) {
			return false;
		}

		const auto editorId = formIdIt->second;

		_formIdToEditorIdMap.erase(formIdIt);
		editorIdMap->erase(editorId);

		return true;
	}

	void GameEditorIds::LogDuplicateEditorId(RE::TESFormID a_ownerFormId, RE::TESFormID a_senderFormId, const RE::BSFixedString& a_editorId) const
	{
		if (a_ownerFormId == a_senderFormId || !_settingsManager->GetLogDuplicateEditorIdsSetting()->GetValue()) {
			return;
		}

		REX::LogWarning(R"(Found duplicate ID; Owner = {:08X}, Sender = {:08X}, ID = "{}")"sv,
			a_ownerFormId, a_senderFormId, a_editorId);
	}
}
