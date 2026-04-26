#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class CellLoadHandler final
		: public EventHandler<
			  CellLoadHandler,
			  CellLoadArgs,
			  CellLoadParams,
			  "OnCellLoad">,
		  public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>
	{
	public:
		CellLoadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~CellLoadHandler() noexcept override;

		CellLoadHandler(const CellLoadHandler&) = delete;
		CellLoadHandler(CellLoadHandler&&) = delete;

		CellLoadHandler& operator=(const CellLoadHandler&) = delete;
		CellLoadHandler& operator=(CellLoadHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceCellFilter() const noexcept { return _sourceCellFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESCellFullyLoadedEvent& a_event,
			RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceCellFilter;
	};
}
