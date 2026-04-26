#include "Plugin/Internal/Events/Handlers/Save/StartGameHandler.hpp"

namespace Plugin::Internal::Events
{
	StartGameHandler::StartGameHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	StartGameHandler::~StartGameHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl StartGameHandler::ProcessEvent(const StartGameEvent& a_event,
		RE::BSTEventSource<StartGameEvent>* a_eventSource)
	{
		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("bSucceeded"sv, a_event.isSucceeded);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void StartGameHandler::RegisterEvents()
	{
		StartGameSource::GetEventSource()->RegisterSink(this);
	}

	void StartGameHandler::UnregisterEvents()
	{
		StartGameSource::GetEventSource()->UnregisterSink(this);
	}
}
