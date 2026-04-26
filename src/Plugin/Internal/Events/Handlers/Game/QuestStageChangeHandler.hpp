#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class QuestStageChangeHandler final
		: public EventHandler<
			  QuestStageChangeHandler,
			  QuestStageChangeArgs,
			  QuestStageChangeParams,
			  "OnQuestStageChange">,
		  public RE::BSTEventSink<RE::TESQuestStageEvent>,
		  public RE::BSTEventSink<RE::TESQuestStageItemDoneEvent>
	{
	public:
		QuestStageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~QuestStageChangeHandler() noexcept override;

		QuestStageChangeHandler(const QuestStageChangeHandler&) = delete;
		QuestStageChangeHandler(QuestStageChangeHandler&&) = delete;

		QuestStageChangeHandler& operator=(const QuestStageChangeHandler&) = delete;
		QuestStageChangeHandler& operator=(QuestStageChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceQuestFilter() const noexcept { return _sourceQuestFilter; }
		[[nodiscard]] const auto& GetNewStageIdFilter() const noexcept { return _newStageIdFilter; }
		[[nodiscard]] const auto& GetNewItemIdFilter() const noexcept { return _newItemIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESQuestStageEvent& a_event,
			RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;

		RE::BSEventNotifyControl ProcessEvent(const RE::TESQuestStageItemDoneEvent& a_event,
			RE::BSTEventSource<RE::TESQuestStageItemDoneEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		bool ProcessEventImpl(RE::TESFormID a_questFormID, std::uint16_t a_stageId, std::uint8_t a_itemId, bool a_isCompleted);

		Script::ScriptVariantFilter _sourceQuestFilter;
		Script::ScriptValueFilter<std::uint16_t> _newStageIdFilter;
		Script::ScriptValueFilter<std::uint16_t> _newItemIdFilter;
	};
}
