#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/StartGameSource.hpp"

namespace Plugin::Internal::Events
{
	class StartGameHandler final
		: public EventHandler<
			  StartGameHandler,
			  StartGameArgs,
			  StartGameParams,
			  "OnStartGame">,
		  public RE::BSTEventSink<StartGameEvent>
	{
	public:
		StartGameHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~StartGameHandler() noexcept override;

		StartGameHandler(const StartGameHandler&) = delete;
		StartGameHandler(StartGameHandler&&) = delete;

		StartGameHandler& operator=(const StartGameHandler&) = delete;
		StartGameHandler& operator=(StartGameHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const StartGameEvent& a_event,
			RE::BSTEventSource<StartGameEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
