#include "Plugin/Internal/Events/Handlers/Game/MenuModeEnterExitHandler.hpp"

namespace Plugin::Internal::Events
{
	MenuModeEnterExitHandler::MenuModeEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _menuNameFilter(a_args, "kIncludedMenuNames"sv, "kExcludedMenuNames"sv)
	{
		RegisterEvents();
	}

	MenuModeEnterExitHandler::~MenuModeEnterExitHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl MenuModeEnterExitHandler::ProcessEvent(const RE::MenuModeChangeEvent& a_event,
		RE::BSTEventSource<RE::MenuModeChangeEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto& menuName = a_event.menuName;

		if (!_menuNameFilter.IsMatch(menuName)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("sMenuName"sv, menuName);
		params.Insert("bEntered"sv, a_event.enteringMenuMode);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void MenuModeEnterExitHandler::RegisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuModeChangeEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->RegisterSink(this);
	}

	void MenuModeEnterExitHandler::UnregisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuModeChangeEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->UnregisterSink(this);
	}
}
