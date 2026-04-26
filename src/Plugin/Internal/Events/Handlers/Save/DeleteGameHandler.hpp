#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/DeleteGameSource.hpp"

namespace Plugin::Internal::Events
{
	class DeleteGameHandler final
		: public EventHandler<
			  DeleteGameHandler,
			  DeleteGameArgs,
			  DeleteGameParams,
			  "OnDeleteGame">,
		  public RE::BSTEventSink<DeleteGameEvent>
	{
	public:
		DeleteGameHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~DeleteGameHandler() noexcept override;

		DeleteGameHandler(const DeleteGameHandler&) = delete;
		DeleteGameHandler(DeleteGameHandler&&) = delete;

		DeleteGameHandler& operator=(const DeleteGameHandler&) = delete;
		DeleteGameHandler& operator=(DeleteGameHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const DeleteGameEvent& a_event,
			RE::BSTEventSource<DeleteGameEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
