#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/NewGameSource.hpp"

namespace Plugin::Internal::Events
{
	class NewGameHandler final
		: public EventHandler<
			  NewGameHandler,
			  NewGameArgs,
			  NewGameParams,
			  "OnNewGame">,
		  public RE::BSTEventSink<NewGameEvent>
	{
	public:
		NewGameHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~NewGameHandler() noexcept override;

		NewGameHandler(const NewGameHandler&) = delete;
		NewGameHandler(NewGameHandler&&) = delete;

		NewGameHandler& operator=(const NewGameHandler&) = delete;
		NewGameHandler& operator=(NewGameHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const NewGameEvent& a_event,
			RE::BSTEventSource<NewGameEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
