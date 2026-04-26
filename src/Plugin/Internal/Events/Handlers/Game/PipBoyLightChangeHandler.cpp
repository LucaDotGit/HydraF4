#include "Plugin/Internal/Events/Handlers/Game/PipBoyLightChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	PipBoyLightChangeHandler::PipBoyLightChangeHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	PipBoyLightChangeHandler::~PipBoyLightChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PipBoyLightChangeHandler::ProcessEvent(const RE::PipboyLightEvent& a_event,
		RE::BSTEventSource<RE::PipboyLightEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("bEnabled"sv, a_event.optionalValue.value_or(false));

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void PipBoyLightChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::PipboyLightEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void PipBoyLightChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::PipboyLightEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
