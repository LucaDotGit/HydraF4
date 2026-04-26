#include "Plugin/Internal/Events/Handlers/Actor/LifeStateChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	LifeStateChangeHandler::LifeStateChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _oldStateFilter(a_args, "kIncludedOldStates"sv, "kExcludedOldStates"sv),
		  _newStateFilter(a_args, "kIncludedNewStates"sv, "kExcludedNewStates"sv)
	{
		RegisterEvents();
	}

	LifeStateChangeHandler::~LifeStateChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LifeStateChangeHandler::ProcessEvent(const LifeStateChangeEvent& a_event,
		RE::BSTEventSource<LifeStateChangeEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceActor = a_event.actor.get();
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto oldState = a_event.oldState.get();
		const auto newState = a_event.newState.get();

		if (!_sourceActorFilter.IsMatch(sourceActor) ||
			!_oldStateFilter.IsMatch(oldState) ||
			!_newStateFilter.IsMatch(newState)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("iOldState"sv, oldState);
		params.Insert("iNewState"sv, newState);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LifeStateChangeHandler::RegisterEvents()
	{
		LifeStateChangeSource::GetEventSource()->RegisterSink(this);
	}

	void LifeStateChangeHandler::UnregisterEvents()
	{
		LifeStateChangeSource::GetEventSource()->UnregisterSink(this);
	}
}
