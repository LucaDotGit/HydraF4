#include "Plugin/Internal/Events/Handlers/Game/MiscStatChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	MiscStatChangeHandler::MiscStatChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _statIdFilter(a_args, "kIncludedStatIds"sv, "kExcludedStatIds"sv)
	{
		RegisterEvents();
	}

	MiscStatChangeHandler::~MiscStatChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl MiscStatChangeHandler::ProcessEvent(const RE::TESTrackedStatsEvent& a_event,
		RE::BSTEventSource<RE::TESTrackedStatsEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto statId = a_event.statID;

		if (!_statIdFilter.IsMatch(statId)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("sStatId"sv, statId);
		params.Insert("iNewValue"sv, a_event.newValue);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void MiscStatChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESTrackedStatsEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void MiscStatChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESTrackedStatsEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
