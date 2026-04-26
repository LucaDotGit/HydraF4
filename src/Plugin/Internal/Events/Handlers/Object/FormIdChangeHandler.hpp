#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class FormIdChangeHandler final
		: public EventHandler<
			  FormIdChangeHandler,
			  FormIdChangeArgs,
			  FormIdChangeParams,
			  "OnFormIdChange">,
		  public RE::BSTEventSink<RE::TESFormIDRemapEvent>
	{
	public:
		FormIdChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~FormIdChangeHandler() noexcept override;

		FormIdChangeHandler(const FormIdChangeHandler&) = delete;
		FormIdChangeHandler(FormIdChangeHandler&&) = delete;

		FormIdChangeHandler& operator=(const FormIdChangeHandler&) = delete;
		FormIdChangeHandler& operator=(FormIdChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetOldFormIdFilter() const noexcept { return _oldFormIdFilter; }
		[[nodiscard]] const auto& GetNewFormIdFilter() const noexcept { return _newFormIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
			RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::TESFormID> _oldFormIdFilter;
		Script::ScriptValueFilter<RE::TESFormID> _newFormIdFilter;
	};
}
