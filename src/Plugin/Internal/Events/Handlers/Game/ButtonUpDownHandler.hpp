#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/ButtonUpDownSource.hpp"

namespace Plugin::Internal::Events
{
	class ButtonUpDownHandler final
		: public EventHandler<
			  ButtonUpDownHandler,
			  ButtonUpDownArgs,
			  ButtonUpDownParams,
			  "OnButtonUpDown">,
		  public RE::BSTEventSink<ButtonUpDownEvent>
	{
	public:
		ButtonUpDownHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ButtonUpDownHandler() noexcept override;

		ButtonUpDownHandler(const ButtonUpDownHandler&) = delete;
		ButtonUpDownHandler(ButtonUpDownHandler&&) = delete;

		ButtonUpDownHandler& operator=(const ButtonUpDownHandler&) = delete;
		ButtonUpDownHandler& operator=(ButtonUpDownHandler&&) = delete;

		[[nodiscard]] const auto& GetDeviceTypeFilter() const noexcept { return _deviceTypeFilter; }
		[[nodiscard]] const auto& GetButtonCodeFilter() const noexcept { return _buttonCodeFilter; }
		[[nodiscard]] const auto& GetControlNameFilter() const noexcept { return _controlNameFilter; }

		RE::BSEventNotifyControl ProcessEvent(const ButtonUpDownEvent& a_event,
			RE::BSTEventSource<ButtonUpDownEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::INPUT_DEVICE> _deviceTypeFilter;
		Script::ScriptValueFilter<RE::BS_BUTTON_CODE> _buttonCodeFilter;
		Script::ScriptValueFilter<RE::BSFixedString> _controlNameFilter;
	};
}
