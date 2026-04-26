#include "Plugin/Internal/Events/Handlers/Game/BookReadHandler.hpp"

namespace Plugin::Internal::Events
{
	BookReadHandler::BookReadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _bookFilter(a_args, "kIncludedBooks"sv, "kExcludedBooks"sv)
	{
		RegisterEvents();
	}

	BookReadHandler::~BookReadHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl BookReadHandler::ProcessEvent(const RE::TESBookReadEvent& a_event,
		RE::BSTEventSource<RE::TESBookReadEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* bookRef = a_event.bookRef.get();
		if (!bookRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* book = RE::TESForm::FindFormByNumericID<RE::TESObjectBOOK>(a_event.baseFormID);
		if (!book) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!(bookRef ? _bookFilter.IsMatch(bookRef) : _bookFilter.IsMatch(book))) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kBookRef"sv, bookRef);
		params.Insert("kBook"sv, book);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void BookReadHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESBookReadEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void BookReadHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESBookReadEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
