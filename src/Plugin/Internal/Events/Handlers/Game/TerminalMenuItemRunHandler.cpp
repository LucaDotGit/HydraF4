#include "Plugin/Internal/Events/Handlers/Game/TerminalMenuItemRunHandler.hpp"

namespace Plugin::Internal::Events
{
	TerminalMenuItemRunHandler::TerminalMenuItemRunHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _terminalFilter(a_args, "kIncludedTerminals"sv, "kExcludedTerminals"sv),
		  _menuItemIdFilter(a_args, "kIncludedMenuItemIds"sv, "kExcludedMenuItemIds"sv)
	{
		RegisterEvents();
	}

	TerminalMenuItemRunHandler::~TerminalMenuItemRunHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl TerminalMenuItemRunHandler::ProcessEvent(const RE::TerminalMenuItemRunEvent& a_event,
		RE::BSTEventSource<RE::TerminalMenuItemRunEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* terminalRef = a_event.targetRef;
		if (!terminalRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* terminal = RE::TESForm::FindFormByNumericID<RE::BGSTerminal>(a_event.terminalFormID);
		if (!terminal) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto menuItemId = a_event.menuItemID;

		if (!(terminalRef ? _terminalFilter.IsMatch(terminalRef) : _terminalFilter.IsMatch(terminal)) ||
			!_menuItemIdFilter.IsMatch(menuItemId)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kTerminalRef"sv, terminalRef);
		params.Insert("kTerminal"sv, terminal);
		params.Insert("iMenuItemID"sv, menuItemId);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void TerminalMenuItemRunHandler::RegisterEvents()
	{
		auto* eventSource = RE::TerminalMenuItemRunEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void TerminalMenuItemRunHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TerminalMenuItemRunEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
