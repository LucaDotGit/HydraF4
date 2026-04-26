#include "Plugin/Internal/Events/Handlers/Actor/CombatStateChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	CombatStateChangeHandler::CombatStateChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetActorFilter(a_args, "kIncludedTargetActors"sv, "kExcludedTargetActors"sv),
		  _newStateFilter(a_args, "kIncludedNewStates"sv, "kExcludedNewStates"sv)
	{
		RegisterEvents();
	}

	CombatStateChangeHandler::~CombatStateChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CombatStateChangeHandler::ProcessEvent(const RE::TESCombatEvent& a_event,
		RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.sourceActorRef);
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.targetActorRef);

		const auto newState = a_event.newState.get();

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_targetActorFilter.IsMatch(targetActor.get()) ||
			!_newStateFilter.IsMatch(newState)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetActor"sv, targetActor);
		params.Insert("iNewState"sv, newState);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void CombatStateChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESCombatEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void CombatStateChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESCombatEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
