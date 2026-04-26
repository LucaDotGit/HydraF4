#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class MenuModeEnterExitHandler final
		: public EventHandler<
			  MenuModeEnterExitHandler,
			  MenuModeEnterExitArgs,
			  MenuModeEnterExitParams,
			  "OnMenuModeEnterExit">,
		  public RE::BSTEventSink<RE::MenuModeChangeEvent>
	{
	public:
		MenuModeEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~MenuModeEnterExitHandler() noexcept override;

		MenuModeEnterExitHandler(const MenuModeEnterExitHandler&) = delete;
		MenuModeEnterExitHandler(MenuModeEnterExitHandler&&) = delete;

		MenuModeEnterExitHandler& operator=(const MenuModeEnterExitHandler&) = delete;
		MenuModeEnterExitHandler& operator=(MenuModeEnterExitHandler&&) = delete;

		[[nodiscard]] const auto& GetMenuNameFilter() const noexcept { return _menuNameFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::MenuModeChangeEvent& a_event,
			RE::BSTEventSource<RE::MenuModeChangeEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::BSFixedString> _menuNameFilter;
	};
}
