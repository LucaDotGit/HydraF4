#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class PerkPointIncreaseHandler final
		: public EventHandler<
			  PerkPointIncreaseHandler,
			  PerkPointIncreaseArgs,
			  PerkPointIncreaseParams,
			  "OnPerkPointIncrease">,
		  public RE::BSTEventSink<RE::PerkPointIncreaseEvent>
	{
	public:
		PerkPointIncreaseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PerkPointIncreaseHandler() noexcept override;

		PerkPointIncreaseHandler(const PerkPointIncreaseHandler&) = delete;
		PerkPointIncreaseHandler(PerkPointIncreaseHandler&&) = delete;

		PerkPointIncreaseHandler& operator=(const PerkPointIncreaseHandler&) = delete;
		PerkPointIncreaseHandler& operator=(PerkPointIncreaseHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::PerkPointIncreaseEvent& a_event,
			RE::BSTEventSource<RE::PerkPointIncreaseEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
