#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class WaitStartStopHandler final
		: public EventHandler<
			  WaitStartStopHandler,
			  WaitStartStopArgs,
			  WaitStartStopParams,
			  "OnWaitStartStop">,
		  public RE::BSTEventSink<RE::TESWaitStartEvent>,
		  public RE::BSTEventSink<RE::TESWaitStopEvent>
	{
	public:
		WaitStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~WaitStartStopHandler() noexcept override;

		WaitStartStopHandler(const WaitStartStopHandler&) = delete;
		WaitStartStopHandler(WaitStartStopHandler&&) = delete;

		WaitStartStopHandler& operator=(const WaitStartStopHandler&) = delete;
		WaitStartStopHandler& operator=(WaitStartStopHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::TESWaitStartEvent& a_event,
			RE::BSTEventSource<RE::TESWaitStartEvent>* a_eventSource) override;

		RE::BSEventNotifyControl ProcessEvent(const RE::TESWaitStopEvent& a_event,
			RE::BSTEventSource<RE::TESWaitStopEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
