#include "Plugin/Internal/Events/Handlers/Game/PerkPointIncreaseHandler.hpp"

namespace Plugin::Internal::Events
{
	PerkPointIncreaseHandler::PerkPointIncreaseHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	PerkPointIncreaseHandler::~PerkPointIncreaseHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PerkPointIncreaseHandler::ProcessEvent(const RE::PerkPointIncreaseEvent& a_event,
		RE::BSTEventSource<RE::PerkPointIncreaseEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iNewCount"sv, a_event.newCount);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void PerkPointIncreaseHandler::RegisterEvents()
	{
		auto* eventSource = RE::PerkPointIncreaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void PerkPointIncreaseHandler::UnregisterEvents()
	{
		auto* eventSource = RE::PerkPointIncreaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
