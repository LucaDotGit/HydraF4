#include "Plugin/Internal/Events/Handlers/Game/HudColorUpdateHandler.hpp"

#include "Plugin/Internal/Structs/Colors.hpp"

namespace Plugin::Internal::Events
{
	HudColorUpdateHandler::HudColorUpdateHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	HudColorUpdateHandler::~HudColorUpdateHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl HudColorUpdateHandler::ProcessEvent(const HudColorUpdateEvent& a_event,
		RE::BSTEventSource<HudColorUpdateEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto color = Structs::Colors::ColorData{
			.red = a_event.red,
			.green = a_event.green,
			.blue = a_event.blue
		};

		auto params = EventParams::Create();

		params.Insert("kNewColor"sv, color);
		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void HudColorUpdateHandler::RegisterEvents()
	{
		HudColorUpdateSource::GetEventSource()->RegisterSink(this);
	}

	void HudColorUpdateHandler::UnregisterEvents()
	{
		HudColorUpdateSource::GetEventSource()->UnregisterSink(this);
	}
}
