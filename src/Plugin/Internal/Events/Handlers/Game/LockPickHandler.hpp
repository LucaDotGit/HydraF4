#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class LockPickHandler final
		: public EventHandler<
			  LockPickHandler,
			  LockPickArgs,
			  LockPickParams,
			  "OnLockPick">,
		  public RE::BSTEventSink<RE::LocksPicked::Event>
	{
	public:
		LockPickHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LockPickHandler() noexcept override;

		LockPickHandler(const LockPickHandler&) = delete;
		LockPickHandler(LockPickHandler&&) = delete;

		LockPickHandler& operator=(const LockPickHandler&) = delete;
		LockPickHandler& operator=(LockPickHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::LocksPicked::Event& a_event,
			RE::BSTEventSource<RE::LocksPicked::Event>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
