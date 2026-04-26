#include "Plugin/Internal/Events/Handlers/Actor/LocationEnterExitHandler.hpp"

namespace Plugin::Internal::Events
{
	LocationEnterExitHandler::LocationEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _oldLocationFilter(a_args, "kIncludedOldLocations"sv, "kExcludedOldLocations"sv),
		  _newLocationFilter(a_args, "kIncludedNewLocations"sv, "kExcludedNewLocations"sv)
	{
		RegisterEvents();
	}

	LocationEnterExitHandler::~LocationEnterExitHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LocationEnterExitHandler::ProcessEvent(const RE::TESActorLocationChangeEvent& a_event,
		RE::BSTEventSource<RE::TESActorLocationChangeEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.actorRef);
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* oldLocation = a_event.oldLocation;
		const auto* newLocation = a_event.newLocation;

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_oldLocationFilter.IsMatch(oldLocation) ||
			!_newLocationFilter.IsMatch(newLocation)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kOldLocation"sv, oldLocation);
		params.Insert("kNewLocation"sv, newLocation);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LocationEnterExitHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESActorLocationChangeEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void LocationEnterExitHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESActorLocationChangeEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
