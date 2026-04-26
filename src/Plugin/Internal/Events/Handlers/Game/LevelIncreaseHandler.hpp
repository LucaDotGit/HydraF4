#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class LevelIncreaseHandler final
		: public EventHandler<
			  LevelIncreaseHandler,
			  LevelIncreaseArgs,
			  LevelIncreaseParams,
			  "OnLevelIncrease">,
		  public RE::BSTEventSink<RE::LevelIncrease::Event>
	{
	public:
		LevelIncreaseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LevelIncreaseHandler() noexcept override;

		LevelIncreaseHandler(const LevelIncreaseHandler&) = delete;
		LevelIncreaseHandler(LevelIncreaseHandler&&) = delete;

		LevelIncreaseHandler& operator=(const LevelIncreaseHandler&) = delete;
		LevelIncreaseHandler& operator=(LevelIncreaseHandler&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::LevelIncrease::Event& a_event,
			RE::BSTEventSource<RE::LevelIncrease::Event>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();
	};
}
