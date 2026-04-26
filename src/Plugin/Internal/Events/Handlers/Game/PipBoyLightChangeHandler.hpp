#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class PipBoyLightChangeHandler final
		: public EventHandler<
			  PipBoyLightChangeHandler,
			  PipBoyLightChangeArgs,
			  PipBoyLightChangeParams,
			  "OnPipBoyLightChange">,
		  public RE::BSTEventSink<RE::PipboyLightEvent>
	{
	public:
		PipBoyLightChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PipBoyLightChangeHandler() noexcept override;

		PipBoyLightChangeHandler(const PipBoyLightChangeHandler&) = delete;
		PipBoyLightChangeHandler(PipBoyLightChangeHandler&&) = delete;

		PipBoyLightChangeHandler& operator=(const PipBoyLightChangeHandler&) = delete;
		PipBoyLightChangeHandler& operator=(PipBoyLightChangeHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event,
			RE::BSTEventSource<RE::PipboyLightEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
