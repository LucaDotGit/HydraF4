#include "Plugin/Internal/Events/Handlers/Game/TutorialTriggerHandler.hpp"

namespace Plugin::Internal::Events
{
	TutorialTriggerHandler::TutorialTriggerHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _eventNameFilter(a_args, "kIncludedEventNames"sv, "kExcludedEventNames"sv)
	{
		RegisterEvents();
	}

	TutorialTriggerHandler::~TutorialTriggerHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl TutorialTriggerHandler::ProcessEvent(const RE::TutorialEvent& a_event,
		RE::BSTEventSource<RE::TutorialEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto& eventName = a_event.eventName;

		if (!_eventNameFilter.IsMatch(eventName)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("sEventName"sv, eventName);
		params.Insert("kSentMessage"sv, a_event.message);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void TutorialTriggerHandler::RegisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::TutorialEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->RegisterSink(this);
	}

	void TutorialTriggerHandler::UnregisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::TutorialEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->UnregisterSink(this);
	}
}
