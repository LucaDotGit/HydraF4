#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class CellEnterExitHandler final
		: public EventHandler<
			  CellEnterExitHandler,
			  CellEnterExitArgs,
			  CellEnterExitParams,
			  "OnCellEnterExit">,
		  public RE::BSTEventSink<RE::BGSActorCellEvent>
	{
	public:
		CellEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~CellEnterExitHandler() noexcept override;

		CellEnterExitHandler(const CellEnterExitHandler&) = delete;
		CellEnterExitHandler(CellEnterExitHandler&&) = delete;

		CellEnterExitHandler& operator=(const CellEnterExitHandler&) = delete;
		CellEnterExitHandler& operator=(CellEnterExitHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetCells() const noexcept { return _targetCellFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent& a_event,
			RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetCellFilter;
	};
}
