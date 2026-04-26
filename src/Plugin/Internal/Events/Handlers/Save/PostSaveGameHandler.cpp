#include "Plugin/Internal/Events/Handlers/Save/PostSaveGameHandler.hpp"

namespace Plugin::Internal::Events
{
	PostSaveGameHandler::PostSaveGameHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	PostSaveGameHandler::~PostSaveGameHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PostSaveGameHandler::ProcessEvent(const PostSaveGameEvent& a_event,
		RE::BSTEventSource<PostSaveGameEvent>* a_eventSource)
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

	void PostSaveGameHandler::RegisterEvents()
	{
		PostSaveGameSource::GetEventSource()->RegisterSink(this);
	}

	void PostSaveGameHandler::UnregisterEvents()
	{
		PostSaveGameSource::GetEventSource()->UnregisterSink(this);
	}
}
