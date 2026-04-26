#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class TutorialTriggerHandler final
		: public EventHandler<
			  TutorialTriggerHandler,
			  TutorialTriggerArgs,
			  TutorialTriggerParams,
			  "OnTutorialTrigger">,
		  public RE::BSTEventSink<RE::TutorialEvent>
	{
	public:
		TutorialTriggerHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~TutorialTriggerHandler() noexcept override;

		TutorialTriggerHandler(const TutorialTriggerHandler&) = delete;
		TutorialTriggerHandler(TutorialTriggerHandler&&) = delete;

		TutorialTriggerHandler& operator=(const TutorialTriggerHandler&) = delete;
		TutorialTriggerHandler& operator=(TutorialTriggerHandler&&) = delete;

		[[nodiscard]] const auto& GetEventNameFilter() const noexcept { return _eventNameFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TutorialEvent& a_event,
			RE::BSTEventSource<RE::TutorialEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::BSFixedString> _eventNameFilter;
	};
}