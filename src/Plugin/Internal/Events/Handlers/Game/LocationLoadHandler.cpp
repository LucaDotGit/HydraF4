#include "Plugin/Internal/Events/Handlers/Game/LocationLoadHandler.hpp"

namespace Plugin::Internal::Events
{
	LocationLoadHandler::LocationLoadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceLocationFilter(a_args, "kIncludedSourceLocations"sv, "kExcludedSourceLocations"sv)
	{
		RegisterEvents();
	}

	LocationLoadHandler::~LocationLoadHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LocationLoadHandler::ProcessEvent(const RE::BGSLocationLoadedEvent& a_event,
		RE::BSTEventSource<RE::BGSLocationLoadedEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceLocation = a_event.location;
		if (!sourceLocation) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceLocationFilter.IsMatch(sourceLocation)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceLocation"sv, sourceLocation);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LocationLoadHandler::RegisterEvents()
	{
		auto* eventSource = RE::BGSLocationLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void LocationLoadHandler::UnregisterEvents()
	{
		auto* eventSource = RE::BGSLocationLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
