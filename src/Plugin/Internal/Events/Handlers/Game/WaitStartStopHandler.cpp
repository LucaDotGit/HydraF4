#include "Plugin/Internal/Events/Handlers/Game/WaitStartStopHandler.hpp"

namespace Plugin::Internal::Events
{
	WaitStartStopHandler::WaitStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	WaitStartStopHandler::~WaitStartStopHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl WaitStartStopHandler::ProcessEvent(const RE::TESWaitStartEvent& a_event,
		RE::BSTEventSource<RE::TESWaitStartEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kStartTime"sv, a_event.startTime);
		params.Insert("kDesiredEndTime"sv, a_event.desiredEndTime);
		params.Insert("bStarted"sv, true);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl WaitStartStopHandler::ProcessEvent(const RE::TESWaitStopEvent& a_event,
		RE::BSTEventSource<RE::TESWaitStopEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kIsInterrupted"sv, a_event.interrupted);
		params.Insert("bStarted"sv, false);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void WaitStartStopHandler::RegisterEvents()
	{
		auto* waitStartEventSource = RE::TESWaitStartEvent::GetEventSource();
		if (waitStartEventSource) {
			waitStartEventSource->RegisterSink(this);
		}

		auto* waitStopEventSource = RE::TESWaitStopEvent::GetEventSource();
		if (waitStopEventSource) {
			waitStopEventSource->RegisterSink(this);
		}
	}

	void WaitStartStopHandler::UnregisterEvents()
	{
		auto* waitStartEventSource = RE::TESWaitStartEvent::GetEventSource();
		if (waitStartEventSource) {
			waitStartEventSource->UnregisterSink(this);
		}

		auto* waitStopEventSource = RE::TESWaitStopEvent::GetEventSource();
		if (waitStopEventSource) {
			waitStopEventSource->UnregisterSink(this);
		}
	}
}
