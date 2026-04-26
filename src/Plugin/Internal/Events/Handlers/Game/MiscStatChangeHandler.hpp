#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class MiscStatChangeHandler final
		: public EventHandler<
			  MiscStatChangeHandler,
			  MiscStatChangeArgs,
			  MiscStatChangeParams,
			  "OnMiscStatChange">,
		  public RE::BSTEventSink<RE::TESTrackedStatsEvent>
	{
	public:
		MiscStatChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~MiscStatChangeHandler() noexcept override;

		MiscStatChangeHandler(const MiscStatChangeHandler&) = delete;
		MiscStatChangeHandler(MiscStatChangeHandler&&) = delete;

		MiscStatChangeHandler& operator=(const MiscStatChangeHandler&) = delete;
		MiscStatChangeHandler& operator=(MiscStatChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetStatIdFilter() const noexcept { return _statIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESTrackedStatsEvent& a_event,
			RE::BSTEventSource<RE::TESTrackedStatsEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptValueFilter<RE::BSFixedString> _statIdFilter;
	};
}
