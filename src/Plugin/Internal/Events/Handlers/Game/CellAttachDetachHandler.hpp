#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class CellAttachDetachHandler final
		: public EventHandler<
			  CellAttachDetachHandler,
			  CellAttachDetachArgs,
			  CellAttachDetachParams,
			  "OnCellAttachDetach">,
		  public RE::BSTEventSink<RE::CellAttachDetachEvent>
	{
	public:
		CellAttachDetachHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~CellAttachDetachHandler() noexcept override;

		CellAttachDetachHandler(const CellAttachDetachHandler&) = delete;
		CellAttachDetachHandler(CellAttachDetachHandler&&) = delete;

		CellAttachDetachHandler& operator=(const CellAttachDetachHandler&) = delete;
		CellAttachDetachHandler& operator=(CellAttachDetachHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceCellFilter() const noexcept { return _sourceCellFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event,
			RE::BSTEventSource<RE::CellAttachDetachEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceCellFilter;
	};
}
