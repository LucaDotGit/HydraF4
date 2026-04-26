#include "Plugin/Internal/Events/Handlers/Game/LevelIncreaseHandler.hpp"

namespace Plugin::Internal::Events
{
	LevelIncreaseHandler::LevelIncreaseHandler(const Script::ScriptFunctionRef& a_functionRef, [[maybe_unused]] const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent)
	{
		RegisterEvents();
	}

	LevelIncreaseHandler::~LevelIncreaseHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LevelIncreaseHandler::ProcessEvent(const RE::LevelIncrease::Event& a_event,
		RE::BSTEventSource<RE::LevelIncrease::Event>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iNewLevel"sv, a_event.newLevel);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LevelIncreaseHandler::RegisterEvents()
	{
		auto* eventSource = RE::LevelIncrease::Event::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void LevelIncreaseHandler::UnregisterEvents()
	{
		auto* eventSource = RE::LevelIncrease::Event::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
