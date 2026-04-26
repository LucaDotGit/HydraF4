#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class MenuOpenCloseHandler final
		: public EventHandler<
			  MenuOpenCloseHandler,
			  MenuOpenCloseArgs,
			  MenuOpenCloseParams,
			  "OnMenuOpenClose">,
		  public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		MenuOpenCloseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~MenuOpenCloseHandler() noexcept override;

		MenuOpenCloseHandler(const MenuOpenCloseHandler&) = delete;
		MenuOpenCloseHandler(MenuOpenCloseHandler&&) = delete;

		MenuOpenCloseHandler& operator=(const MenuOpenCloseHandler&) = delete;
		MenuOpenCloseHandler& operator=(MenuOpenCloseHandler&&) = delete;

		[[nodiscard]] const auto& GetMenuNameFilter() const noexcept { return _menuNameFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event,
			RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::BSFixedString> _menuNameFilter;
	};
}
