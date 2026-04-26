#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/HudColorUpdateSource.hpp"

namespace Plugin::Internal::Events
{
	class HudColorUpdateHandler final
		: public EventHandler<
			  HudColorUpdateHandler,
			  HudColorUpdateArgs,
			  HudColorUpdateParams,
			  "OnHudColorUpdate">,
		  public RE::BSTEventSink<HudColorUpdateEvent>
	{
	public:
		HudColorUpdateHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~HudColorUpdateHandler() noexcept override;

		HudColorUpdateHandler(const HudColorUpdateHandler&) = delete;
		HudColorUpdateHandler(HudColorUpdateHandler&&) = delete;

		HudColorUpdateHandler& operator=(const HudColorUpdateHandler&) = delete;
		HudColorUpdateHandler& operator=(HudColorUpdateHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const HudColorUpdateEvent& a_event,
			RE::BSTEventSource<HudColorUpdateEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
