#include "Plugin/Internal/Events/Handlers/Save/PostLoadGameHandler.hpp"

namespace Plugin::Internal::Events
{
	PostLoadGameHandler::PostLoadGameHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	PostLoadGameHandler::~PostLoadGameHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PostLoadGameHandler::ProcessEvent(const PostLoadGameEvent& a_event,
		RE::BSTEventSource<PostLoadGameEvent>* a_eventSource)
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

	void PostLoadGameHandler::RegisterEvents()
	{
		PostLoadGameSource::GetEventSource()->RegisterSink(this);
	}

	void PostLoadGameHandler::UnregisterEvents()
	{
		PostLoadGameSource::GetEventSource()->UnregisterSink(this);
	}
}
