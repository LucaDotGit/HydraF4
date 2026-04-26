#include "Plugin/Internal/Events/Handlers/Object/FormDeleteHandler.hpp"

namespace Plugin::Internal::Events
{
	FormDeleteHandler::FormDeleteHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceFormIdFilter(a_args, "kIncludedSourceFormIds"sv, "kExcludedSourceFormIds"sv)
	{
		RegisterEvents();
	}

	FormDeleteHandler::~FormDeleteHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl FormDeleteHandler::ProcessEvent(const RE::TESFormDeleteEvent& a_event,
		RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceFormId = a_event.formID;
		if (sourceFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceFormIdFilter.IsMatch(sourceFormId)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("iSourceFormId"sv, sourceFormId);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void FormDeleteHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void FormDeleteHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
