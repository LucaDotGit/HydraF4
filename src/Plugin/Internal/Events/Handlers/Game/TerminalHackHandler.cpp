#include "Plugin/Internal/Events/Handlers/Game/TerminalHackHandler.hpp"

namespace Plugin::Internal::Events
{
	TerminalHackHandler::TerminalHackHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	TerminalHackHandler::~TerminalHackHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl TerminalHackHandler::ProcessEvent(const RE::TerminalHacked::Event& a_event,
		RE::BSTEventSource<RE::TerminalHacked::Event>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.terminalRef.underlying();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* internalLock = targetRef->GetLock();
		if (!internalLock) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto enumLockLevel = internalLock->GetLockLevel(targetRef);
		const auto numericLockLevel = RE::REFR_LOCK::GetNumericLockLevel(enumLockLevel);

		auto params = EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("iLockLevel"sv, numericLockLevel);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void TerminalHackHandler::RegisterEvents()
	{
		auto* eventSource = RE::TerminalHacked::Event::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void TerminalHackHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TerminalHacked::Event::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
