#include "Plugin/Internal/Events/Handlers/Actor/ActorDeathHandler.hpp"

namespace Plugin::Internal::Events
{
	ActorDeathHandler::ActorDeathHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetActorFilter(a_args, "kIncludedTargetActors"sv, "kExcludedTargetActors"sv)
	{
		RegisterEvents();
	}

	ActorDeathHandler::~ActorDeathHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ActorDeathHandler::ProcessEvent(const RE::TESDeathEvent& a_event,
		RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.dyingActorRef);
		if (!targetActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.killerActorRef);

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_targetActorFilter.IsMatch(targetActor.get())) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetActor"sv, targetActor);
		params.Insert("bDied"sv, !a_event.dying);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ActorDeathHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESDeathEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ActorDeathHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESDeathEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
