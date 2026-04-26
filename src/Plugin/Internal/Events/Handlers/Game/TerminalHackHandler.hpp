#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class TerminalHackHandler final
		: public EventHandler<
			  TerminalHackHandler,
			  TerminalHackArgs,
			  TerminalHackParams,
			  "OnTerminalHack">,
		  public RE::BSTEventSink<RE::TerminalHacked::Event>
	{
	public:
		TerminalHackHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~TerminalHackHandler() noexcept override;

		TerminalHackHandler(const TerminalHackHandler&) = delete;
		TerminalHackHandler(TerminalHackHandler&&) = delete;

		TerminalHackHandler& operator=(const TerminalHackHandler&) = delete;
		TerminalHackHandler& operator=(TerminalHackHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TerminalHacked::Event& a_event,
			RE::BSTEventSource<RE::TerminalHacked::Event>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
