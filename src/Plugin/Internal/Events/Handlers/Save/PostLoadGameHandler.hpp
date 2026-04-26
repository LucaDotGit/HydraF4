#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/PostLoadGameSource.hpp"

namespace Plugin::Internal::Events
{
	class PostLoadGameHandler final
		: public EventHandler<
			  PostLoadGameHandler,
			  PostLoadGameArgs,
			  PostLoadGameParams,
			  "OnPostLoadGame">,
		  public RE::BSTEventSink<PostLoadGameEvent>
	{
	public:
		PostLoadGameHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PostLoadGameHandler() noexcept override;

		PostLoadGameHandler(const PostLoadGameHandler&) = delete;
		PostLoadGameHandler(PostLoadGameHandler&&) = delete;

		PostLoadGameHandler& operator=(const PostLoadGameHandler&) = delete;
		PostLoadGameHandler& operator=(PostLoadGameHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const PostLoadGameEvent& a_event,
			RE::BSTEventSource<PostLoadGameEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
