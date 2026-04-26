#include "Plugin/Internal/Events/Handlers/Game/QuestStartStopHandler.hpp"

namespace Plugin::Internal::Events
{
	QuestStartStopHandler::QuestStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceQuestFilter(a_args, "kIncludedSourceQuests"sv, "kExcludedSourceQuests"sv)
	{
		RegisterEvents();
	}

	QuestStartStopHandler::~QuestStartStopHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl QuestStartStopHandler::ProcessEvent(const RE::TESQuestStartStopEvent& a_event,
		RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceQuest = RE::TESForm::FindFormByNumericID<RE::TESQuest>(a_event.questFormID);
		if (!sourceQuest) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceQuestFilter.IsMatch(sourceQuest)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceQuest"sv, sourceQuest);
		params.Insert("bStarted"sv, a_event.started);
		params.Insert("bFailed"sv, a_event.failed);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void QuestStartStopHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESQuestStartStopEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void QuestStartStopHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESQuestStartStopEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
