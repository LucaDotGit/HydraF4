#include "Plugin/Internal/Events/Handlers/Save/DeleteGameHandler.hpp"

namespace Plugin::Internal::Events
{
	DeleteGameHandler::DeleteGameHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	DeleteGameHandler::~DeleteGameHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl DeleteGameHandler::ProcessEvent(const DeleteGameEvent& a_event,
		RE::BSTEventSource<DeleteGameEvent>* a_eventSource)
	{
		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("sSaveName"sv, a_event.saveName);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void DeleteGameHandler::RegisterEvents()
	{
		DeleteGameSource::GetEventSource()->RegisterSink(this);
	}

	void DeleteGameHandler::UnregisterEvents()
	{
		DeleteGameSource::GetEventSource()->UnregisterSink(this);
	}
}
