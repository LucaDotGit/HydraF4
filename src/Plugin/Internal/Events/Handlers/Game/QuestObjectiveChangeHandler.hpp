#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class QuestObjectiveChangeHandler final
		: public EventHandler<
			  QuestObjectiveChangeHandler,
			  QuestObjectiveChangeArgs,
			  QuestObjectiveChangeParams,
			  "OnQuestObjectiveChange">,
		  public RE::BSTEventSink<RE::ObjectiveState::Event>
	{
	public:
		QuestObjectiveChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~QuestObjectiveChangeHandler() noexcept override;

		QuestObjectiveChangeHandler(const QuestObjectiveChangeHandler&) = delete;
		QuestObjectiveChangeHandler(QuestObjectiveChangeHandler&&) = delete;

		QuestObjectiveChangeHandler& operator=(const QuestObjectiveChangeHandler&) = delete;
		QuestObjectiveChangeHandler& operator=(QuestObjectiveChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceQuestFilter() const noexcept { return _sourceQuestFilter; }
		[[nodiscard]] const auto& GetNewObjectiveIdFilter() const noexcept { return _newObjectiveIdFilter; }
		[[nodiscard]] const auto& GetOldObjectiveStateFilter() const noexcept { return _oldObjectiveStateFilter; }
		[[nodiscard]] const auto& GetNewObjectiveStateFilter() const noexcept { return _newObjectiveStateFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::ObjectiveState::Event& a_event,
			RE::BSTEventSource<RE::ObjectiveState::Event>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceQuestFilter;
		Script::ScriptValueFilter<std::uint16_t> _newObjectiveIdFilter;
		Script::ScriptValueFilter<RE::QUEST_OBJECTIVE_STATE> _oldObjectiveStateFilter;
		Script::ScriptValueFilter<RE::QUEST_OBJECTIVE_STATE> _newObjectiveStateFilter;
	};
}
