#include "Plugin/Internal/Events/Handlers/Game/PowerArmorLightChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	PowerArmorLightChangeHandler::PowerArmorLightChangeHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	PowerArmorLightChangeHandler::~PowerArmorLightChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PowerArmorLightChangeHandler::ProcessEvent(const RE::PowerArmorLightData& a_event,
		RE::BSTEventSource<RE::PowerArmorLightData>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("bEnabled"sv, a_event.lightOn);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void PowerArmorLightChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::PowerArmorLightData::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void PowerArmorLightChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::PowerArmorLightData::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
