#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class BookReadHandler final
		: public EventHandler<
			  BookReadHandler,
			  BookReadArgs,
			  BookReadParams,
			  "OnBookRead">,
		  public RE::BSTEventSink<RE::TESBookReadEvent>
	{
	public:
		BookReadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~BookReadHandler() noexcept override;

		BookReadHandler(const BookReadHandler&) = delete;
		BookReadHandler(BookReadHandler&&) = delete;

		BookReadHandler& operator=(const BookReadHandler&) = delete;
		BookReadHandler& operator=(BookReadHandler&&) = delete;

		[[nodiscard]] const auto& GetBookFilter() const noexcept { return _bookFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESBookReadEvent& a_event,
			RE::BSTEventSource<RE::TESBookReadEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _bookFilter;
	};
}
