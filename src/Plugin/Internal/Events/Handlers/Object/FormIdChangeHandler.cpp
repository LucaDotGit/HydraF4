#include "Plugin/Internal/Events/Handlers/Object/FormIdChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	FormIdChangeHandler::FormIdChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _oldFormIdFilter(a_args, "kIncludedOldFormIds"sv, "kExcludedOldFormIds"sv),
		  _newFormIdFilter(a_args, "kIncludedNewFormIds"sv, "kExcludedNewFormIds"sv)
	{
		RegisterEvents();
	}

	FormIdChangeHandler::~FormIdChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl FormIdChangeHandler::ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
		RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto oldFormId = a_event.oldFormID;
		if (oldFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto newFormId = a_event.newFormID;
		if (newFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iOldFormId"sv, oldFormId);
		params.Insert("iNewFormId"sv, newFormId);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void FormIdChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void FormIdChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
