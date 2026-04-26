#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class QuestStartStopHandler final
		: public EventHandler<
			  QuestStartStopHandler,
			  QuestStartStopArgs,
			  QuestStartStopParams,
			  "OnQuestStartStop">,
		  public RE::BSTEventSink<RE::TESQuestStartStopEvent>
	{
	public:
		QuestStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~QuestStartStopHandler() noexcept override;

		QuestStartStopHandler(const QuestStartStopHandler&) = delete;
		QuestStartStopHandler(QuestStartStopHandler&&) = delete;

		QuestStartStopHandler& operator=(const QuestStartStopHandler&) = delete;
		QuestStartStopHandler& operator=(QuestStartStopHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceQuestFilter() const noexcept { return _sourceQuestFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESQuestStartStopEvent& a_event,
			RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceQuestFilter;
	};
}
