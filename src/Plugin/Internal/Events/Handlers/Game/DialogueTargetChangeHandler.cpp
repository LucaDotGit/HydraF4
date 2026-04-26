#include "Plugin/Internal/Events/Handlers/Game/DialogueTargetChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	DialogueTargetChangeHandler::DialogueTargetChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	DialogueTargetChangeHandler::~DialogueTargetChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl DialogueTargetChangeHandler::ProcessEvent(const RE::TESOnPCDialogueTargetEvent& a_event,
		RE::BSTEventSource<RE::TESOnPCDialogueTargetEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.targetRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void DialogueTargetChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESOnPCDialogueTargetEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void DialogueTargetChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESOnPCDialogueTargetEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
