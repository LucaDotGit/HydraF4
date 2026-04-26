#include "Plugin/Internal/Events/Handlers/Game/ButtonUpDownHandler.hpp"

namespace Plugin::Internal::Events
{
	ButtonUpDownHandler::ButtonUpDownHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _deviceTypeFilter(a_args, "kIncludedDeviceTypes"sv, "kExcludedDeviceTypes"sv),
		  _buttonCodeFilter(a_args, "kIncludedButtonCodes"sv, "kExcludedButtonCodes"sv),
		  _controlNameFilter(a_args, "kIncludedControlNames"sv, "kExcludedControlNames"sv)
	{
		RegisterEvents();
	}

	ButtonUpDownHandler::~ButtonUpDownHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ButtonUpDownHandler::ProcessEvent(const ButtonUpDownEvent& a_event,
		RE::BSTEventSource<ButtonUpDownEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_event.IsDown() && !a_event.IsUp()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_deviceTypeFilter.IsMatch(a_event.deviceType.get()) ||
			!_buttonCodeFilter.IsMatch(a_event.buttonCode.get()) ||
			!_controlNameFilter.IsMatch(a_event.controlName)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iDeviceType"sv, a_event.deviceType.get());
		params.Insert("iButtonCode"sv, a_event.buttonCode.get());
		params.Insert("sControlName"sv, a_event.controlName);
		params.Insert("fAnalogValue"sv, a_event.analogValue);
		params.Insert("fHeldSeconds"sv, a_event.heldSeconds);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ButtonUpDownHandler::RegisterEvents()
	{
		ButtonUpDownSource::GetEventSource()->RegisterSink(this);
	}

	void ButtonUpDownHandler::UnregisterEvents()
	{
		ButtonUpDownSource::GetEventSource()->UnregisterSink(this);
	}
}
