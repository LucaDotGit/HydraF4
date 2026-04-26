#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/PostSaveGameSource.hpp"

namespace Plugin::Internal::Events
{
	class PostSaveGameHandler final
		: public EventHandler<
			  PostSaveGameHandler,
			  PostSaveGameArgs,
			  PostSaveGameParams,
			  "OnPostSaveGame">,
		  public RE::BSTEventSink<PostSaveGameEvent>
	{
	public:
		PostSaveGameHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PostSaveGameHandler() noexcept override;

		PostSaveGameHandler(const PostSaveGameHandler&) = delete;
		PostSaveGameHandler(PostSaveGameHandler&&) = delete;

		PostSaveGameHandler& operator=(const PostSaveGameHandler&) = delete;
		PostSaveGameHandler& operator=(PostSaveGameHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const PostSaveGameEvent& a_event,
			RE::BSTEventSource<PostSaveGameEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
