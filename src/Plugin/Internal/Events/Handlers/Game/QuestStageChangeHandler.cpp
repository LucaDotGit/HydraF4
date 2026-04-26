#include "Plugin/Internal/Events/Handlers/Game/QuestStageChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	QuestStageChangeHandler::QuestStageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceQuestFilter(a_args, "kIncludedSourceQuests"sv, "kExcludedSourceQuests"sv),
		  _newStageIdFilter(a_args, "kIncludedNewStageIds"sv, "kExcludedNewStageIds"sv),
		  _newItemIdFilter(a_args, "kIncludedNewItemIds"sv, "kExcludedNewItemIds"sv)
	{
		RegisterEvents();
	}

	QuestStageChangeHandler::~QuestStageChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl QuestStageChangeHandler::ProcessEvent(const RE::TESQuestStageEvent& a_event,
		RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!ProcessEventImpl(a_event.questFormID, a_event.stageID, a_event.itemID, false)) [[unlikely]] {
			REX::Assert(false);
			a_eventSource->UnregisterSink(this);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl QuestStageChangeHandler::ProcessEvent(const RE::TESQuestStageItemDoneEvent& a_event,
		RE::BSTEventSource<RE::TESQuestStageItemDoneEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!ProcessEventImpl(a_event.questFormID, a_event.stageID, a_event.itemID, true)) [[unlikely]] {
			REX::Assert(false);
			a_eventSource->UnregisterSink(this);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	void QuestStageChangeHandler::RegisterEvents()
	{
		auto* questStageEventSource = RE::TESQuestStageEvent::GetEventSource();
		if (questStageEventSource) {
			questStageEventSource->RegisterSink(this);
		}

		auto* questStageItemDoneEventSource = RE::TESQuestStageItemDoneEvent::GetEventSource();
		if (questStageItemDoneEventSource) {
			questStageItemDoneEventSource->RegisterSink(this);
		}
	}

	void QuestStageChangeHandler::UnregisterEvents()
	{
		auto* questStageEventSource = RE::TESQuestStageEvent::GetEventSource();
		if (questStageEventSource) {
			questStageEventSource->UnregisterSink(this);
		}

		auto* questStageItemDoneEventSource = RE::TESQuestStageItemDoneEvent::GetEventSource();
		if (questStageItemDoneEventSource) {
			questStageItemDoneEventSource->UnregisterSink(this);
		}
	}

	bool QuestStageChangeHandler::ProcessEventImpl(RE::TESFormID a_questFormID, std::uint16_t a_stageId, std::uint8_t a_itemId, bool a_isCompleted)
	{
		const auto* sourceQuest = RE::TESForm::FindFormByNumericID<RE::TESQuest>(a_questFormID);
		if (!sourceQuest) [[unlikely]] {
			REX::Assert(false);
			return true;
		}

		if (!_sourceQuestFilter.IsMatch(sourceQuest) ||
			!_newStageIdFilter.IsMatch(a_stageId) ||
			!_newItemIdFilter.IsMatch(a_itemId)) {
			return true;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceQuest"sv, sourceQuest);
		params.Insert("iNewStageId"sv, a_stageId);
		params.Insert("iNewItemId"sv, a_itemId);
		params.Insert("bCompleted"sv, a_isCompleted);

		return _functionRef.InvokeFunction(std::move(params));
	}
}
