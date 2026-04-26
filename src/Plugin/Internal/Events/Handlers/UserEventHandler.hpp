#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/UserEventSource.hpp"

namespace Plugin::Internal::Events
{
	class UserEventHandler final
		: public EventHandler<
			  UserEventHandler,
			  UserEventArgs,
			  UserEventParams,
			  "OnUserEvent">,
		  public RE::BSTEventSink<UserEvent>
	{
	public:
		inline static constexpr auto DISABLE_CALLBACK_VALIDATION = true;

		UserEventHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~UserEventHandler() noexcept override;

		UserEventHandler(const UserEventHandler&) = delete;
		UserEventHandler(UserEventHandler&&) = delete;

		UserEventHandler& operator=(const UserEventHandler&) = delete;
		UserEventHandler& operator=(UserEventHandler&&) = delete;

		[[nodiscard]] const auto& GetEventNameFilter() const noexcept { return _eventNameFilter; }

		RE::BSEventNotifyControl ProcessEvent(const UserEvent& a_event,
			RE::BSTEventSource<UserEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::BSFixedString> _eventNameFilter;
	};
}

namespace Plugin::Internal::Events
{
	template <class>
	struct event_traits;

	template <>
	struct event_traits<UserEventHandler>
	{
		using ValidateCallback = std::false_type;
	};
}
