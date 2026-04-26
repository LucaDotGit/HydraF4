#include "Plugin/Internal/Events/Handlers/Game/DifficultyChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	DifficultyChangeHandler::DifficultyChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _oldDifficultyFilter(a_args, "kIncludedOldDifficulties"sv, "kExcludedOldDifficulties"sv),
		  _newDifficultyFilter(a_args, "kIncludedNewDifficulties"sv, "kExcludedNewDifficulties"sv)
	{
		RegisterEvents();
	}

	DifficultyChangeHandler::~DifficultyChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl DifficultyChangeHandler::ProcessEvent(const RE::PlayerDifficultySettingChanged::Event& a_event,
		RE::BSTEventSource<RE::PlayerDifficultySettingChanged::Event>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto oldDifficulty = a_event.oldDifficulty.get();
		const auto newDifficulty = a_event.newDifficulty.get();

		if (!_oldDifficultyFilter.IsMatch(oldDifficulty) ||
			!_newDifficultyFilter.IsMatch(newDifficulty)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iOldDifficulty"sv, oldDifficulty);
		params.Insert("iNewDifficulty"sv, newDifficulty);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void DifficultyChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::PlayerDifficultySettingChanged::Event::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void DifficultyChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::PlayerDifficultySettingChanged::Event::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
