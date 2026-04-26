#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class TerminalMenuItemRunHandler final
		: public EventHandler<
			  TerminalMenuItemRunHandler,
			  TerminalMenuItemRunArgs,
			  TerminalMenuItemRunParams,
			  "OnTerminalMenuItemRun">,
		  public RE::BSTEventSink<RE::TerminalMenuItemRunEvent>
	{
	public:
		TerminalMenuItemRunHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~TerminalMenuItemRunHandler() noexcept override;

		TerminalMenuItemRunHandler(const TerminalMenuItemRunHandler&) = delete;
		TerminalMenuItemRunHandler(TerminalMenuItemRunHandler&&) = delete;

		TerminalMenuItemRunHandler& operator=(const TerminalMenuItemRunHandler&) = delete;
		TerminalMenuItemRunHandler& operator=(TerminalMenuItemRunHandler&&) = delete;

		[[nodiscard]] const auto& GetTerminalFilter() const noexcept { return _terminalFilter; }
		[[nodiscard]] const auto& GetMenuItemIdFilter() const noexcept { return _menuItemIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TerminalMenuItemRunEvent& a_event,
			RE::BSTEventSource<RE::TerminalMenuItemRunEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _terminalFilter;
		Script::ScriptValueFilter<std::uint32_t> _menuItemIdFilter;
	};
}
