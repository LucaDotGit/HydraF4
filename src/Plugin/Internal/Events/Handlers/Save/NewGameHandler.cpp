#include "Plugin/Internal/Events/Handlers/Save/NewGameHandler.hpp"

namespace Plugin::Internal::Events
{
	NewGameHandler::NewGameHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	NewGameHandler::~NewGameHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl NewGameHandler::ProcessEvent(const NewGameEvent& a_event,
		RE::BSTEventSource<NewGameEvent>* a_eventSource)
	{
		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kCharGenQuest"sv, a_event.charGenQuest);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void NewGameHandler::RegisterEvents()
	{
		NewGameSource::GetEventSource()->RegisterSink(this);
	}

	void NewGameHandler::UnregisterEvents()
	{
		NewGameSource::GetEventSource()->UnregisterSink(this);
	}
}
