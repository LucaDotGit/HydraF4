#include "Plugin/Internal/Events/Handlers/Game/QuestObjectiveChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	QuestObjectiveChangeHandler::QuestObjectiveChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceQuestFilter(a_args, "kIncludedSourceQuests"sv, "kExcludedSourceQuests"sv),
		  _newObjectiveIdFilter(a_args, "kIncludedNewObjectiveIds"sv, "kExcludedNewObjectiveIds"sv),
		  _oldObjectiveStateFilter(a_args, "kIncludedOldObjectiveStates"sv, "kExcludedOldObjectiveStates"sv),
		  _newObjectiveStateFilter(a_args, "kIncludedNewObjectiveStates"sv, "kExcludedNewObjectiveStates"sv)
	{
		RegisterEvents();
	}

	QuestObjectiveChangeHandler::~QuestObjectiveChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl QuestObjectiveChangeHandler::ProcessEvent(const RE::ObjectiveState::Event& a_event,
		RE::BSTEventSource<RE::ObjectiveState::Event>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* newObjective = a_event.objective;
		if (!newObjective) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceQuest = newObjective->ownerQuest;
		if (!sourceQuest) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto newObjectiveId = newObjective->index;
		const auto oldObjectiveState = a_event.oldState.get();
		const auto newObjectiveState = a_event.newState.get();

		if (!_sourceQuestFilter.IsMatch(sourceQuest) ||
			!_newObjectiveIdFilter.IsMatch(newObjectiveId) ||
			!_oldObjectiveStateFilter.IsMatch(oldObjectiveState) ||
			!_newObjectiveStateFilter.IsMatch(newObjectiveState)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceQuest"sv, sourceQuest);
		params.Insert("iNewObjectiveId"s, newObjectiveId);
		params.Insert("kOldObjectiveState"s, oldObjectiveState);
		params.Insert("kNewObjectiveState"s, newObjectiveState);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void QuestObjectiveChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::ObjectiveState::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void QuestObjectiveChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::ObjectiveState::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
