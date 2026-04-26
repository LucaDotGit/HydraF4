#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class DifficultyChangeHandler final
		: public EventHandler<
			  DifficultyChangeHandler,
			  DifficultyChangeArgs,
			  DifficultyChangeParams,
			  "OnDifficultyChange">,
		  public RE::BSTEventSink<RE::PlayerDifficultySettingChanged::Event>
	{
	public:
		DifficultyChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~DifficultyChangeHandler() noexcept override;

		DifficultyChangeHandler(const DifficultyChangeHandler&) = delete;
		DifficultyChangeHandler(DifficultyChangeHandler&&) = delete;

		DifficultyChangeHandler& operator=(const DifficultyChangeHandler&) = delete;
		DifficultyChangeHandler& operator=(DifficultyChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetOldDifficultyFilter() const noexcept { return _oldDifficultyFilter; }
		[[nodiscard]] const auto& GetNewDifficultyFilter() const noexcept { return _newDifficultyFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::PlayerDifficultySettingChanged::Event& a_event,
			RE::BSTEventSource<RE::PlayerDifficultySettingChanged::Event>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::DifficultyLevel> _oldDifficultyFilter;
		Script::ScriptValueFilter<RE::DifficultyLevel> _newDifficultyFilter;
	};
}
