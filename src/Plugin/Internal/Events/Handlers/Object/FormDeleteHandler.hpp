#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class FormDeleteHandler final
		: public EventHandler<
			  FormDeleteHandler,
			  FormDeleteArgs,
			  FormDeleteParams,
			  "OnFormDelete">,
		  public RE::BSTEventSink<RE::TESFormDeleteEvent>
	{
	public:
		FormDeleteHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~FormDeleteHandler() noexcept override;

		FormDeleteHandler(const FormDeleteHandler&) = delete;
		FormDeleteHandler(FormDeleteHandler&&) = delete;

		FormDeleteHandler& operator=(const FormDeleteHandler&) = delete;
		FormDeleteHandler& operator=(FormDeleteHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceFormIdFilter() const noexcept { return _sourceFormIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormDeleteEvent& a_event,
			RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::TESFormID> _sourceFormIdFilter;
	};
}
