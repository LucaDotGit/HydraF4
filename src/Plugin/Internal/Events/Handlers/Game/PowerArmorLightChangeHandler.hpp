#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class PowerArmorLightChangeHandler final
		: public EventHandler<
			  PowerArmorLightChangeHandler,
			  PowerArmorLightChangeArgs,
			  PowerArmorLightChangeParams,
			  "OnPowerArmorLightChange">,
		  public RE::BSTEventSink<RE::PowerArmorLightData>
	{
	public:
		PowerArmorLightChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PowerArmorLightChangeHandler() noexcept override;

		PowerArmorLightChangeHandler(const PowerArmorLightChangeHandler&) = delete;
		PowerArmorLightChangeHandler(PowerArmorLightChangeHandler&&) = delete;

		PowerArmorLightChangeHandler& operator=(const PowerArmorLightChangeHandler&) = delete;
		PowerArmorLightChangeHandler& operator=(PowerArmorLightChangeHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::PowerArmorLightData& a_event,
			RE::BSTEventSource<RE::PowerArmorLightData>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
