#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class CombatStateChangeHandler final
		: public EventHandler<
			  CombatStateChangeHandler,
			  CombatStateChangeArgs,
			  CombatStateChangeParams,
			  "OnCombatStateChange">,
		  public RE::BSTEventSink<RE::TESCombatEvent>
	{
	public:
		CombatStateChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~CombatStateChangeHandler() noexcept override;

		CombatStateChangeHandler(const CombatStateChangeHandler&) = delete;
		CombatStateChangeHandler(CombatStateChangeHandler&&) = delete;

		CombatStateChangeHandler& operator=(const CombatStateChangeHandler&) = delete;
		CombatStateChangeHandler& operator=(CombatStateChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetActorFilter() const noexcept { return _targetActorFilter; }
		[[nodiscard]] const auto& GetNewStateFilter() const noexcept { return _newStateFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent& a_event,
			RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetActorFilter;
		Script::ScriptValueFilter<RE::ACTOR_COMBAT_STATE> _newStateFilter;
	};
}
