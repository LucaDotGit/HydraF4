#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class SleepStartStopHandler final
		: public EventHandler<
			  SleepStartStopHandler,
			  SleepStartStopArgs,
			  SleepStartStopParams,
			  "OnSleepStartStop">,
		  public RE::BSTEventSink<RE::TESSleepStartEvent>,
		  public RE::BSTEventSink<RE::TESSleepStopEvent>
	{
	public:
		SleepStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~SleepStartStopHandler() noexcept override;

		SleepStartStopHandler(const SleepStartStopHandler&) = delete;
		SleepStartStopHandler(SleepStartStopHandler&&) = delete;

		SleepStartStopHandler& operator=(const SleepStartStopHandler&) = delete;
		SleepStartStopHandler& operator=(SleepStartStopHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStartEvent& a_event,
			RE::BSTEventSource<RE::TESSleepStartEvent>* a_eventSource) override;

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent& a_event,
			RE::BSTEventSource<RE::TESSleepStopEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
