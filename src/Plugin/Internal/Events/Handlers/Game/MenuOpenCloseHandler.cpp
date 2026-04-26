#include "Plugin/Internal/Events/Handlers/Game/MenuOpenCloseHandler.hpp"

namespace Plugin::Internal::Events
{
	MenuOpenCloseHandler::MenuOpenCloseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _menuNameFilter(a_args, "kIncludedMenuNames"sv, "kExcludedMenuNames"sv)
	{
		RegisterEvents();
	}

	MenuOpenCloseHandler::~MenuOpenCloseHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl MenuOpenCloseHandler::ProcessEvent(const RE::MenuOpenCloseEvent& a_event,
		RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
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
		params.Insert("bOpened"sv, a_event.opening);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void MenuOpenCloseHandler::RegisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuOpenCloseEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->RegisterSink(this);
	}

	void MenuOpenCloseHandler::UnregisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuOpenCloseEvent>*>(ui);
		if (!eventSource) {
			return;
		}

		eventSource->UnregisterSink(this);
	}
}
