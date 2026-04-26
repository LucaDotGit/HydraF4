#include "Plugin/Internal/Events/Handlers/Game/SleepStartStopHandler.hpp"

namespace Plugin::Internal::Events
{
	SleepStartStopHandler::SleepStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	SleepStartStopHandler::~SleepStartStopHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl SleepStartStopHandler::ProcessEvent(const RE::TESSleepStartEvent& a_event,
		RE::BSTEventSource<RE::TESSleepStartEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.bedRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("kStartTime"sv, a_event.startTime);
		params.Insert("kDesiredEndTime"sv, a_event.desiredEndTime);
		params.Insert("bStarted"sv, true);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl SleepStartStopHandler::ProcessEvent(const RE::TESSleepStopEvent& a_event,
		RE::BSTEventSource<RE::TESSleepStopEvent>* a_eventSource)
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

	void SleepStartStopHandler::RegisterEvents()
	{
		auto* waitStartEventSource = RE::TESSleepStartEvent::GetEventSource();
		if (waitStartEventSource) {
			waitStartEventSource->RegisterSink(this);
		}

		auto* waitStopEventSource = RE::TESSleepStopEvent::GetEventSource();
		if (waitStopEventSource) {
			waitStopEventSource->RegisterSink(this);
		}
	}

	void SleepStartStopHandler::UnregisterEvents()
	{
		auto* waitStartEventSource = RE::TESSleepStartEvent::GetEventSource();
		if (waitStartEventSource) {
			waitStartEventSource->UnregisterSink(this);
		}

		auto* waitStopEventSource = RE::TESSleepStopEvent::GetEventSource();
		if (waitStopEventSource) {
			waitStopEventSource->UnregisterSink(this);
		}
	}
}
