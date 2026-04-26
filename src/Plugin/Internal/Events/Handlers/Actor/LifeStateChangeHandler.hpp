#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/LifeStateChangeSource.hpp"

namespace Plugin::Internal::Events
{
	class LifeStateChangeHandler final
		: public EventHandler<
			  LifeStateChangeHandler,
			  LifeStateChangeArgs,
			  LifeStateChangeParams,
			  "OnLifeStateChange">,
		  public RE::BSTEventSink<LifeStateChangeEvent>
	{
	public:
		LifeStateChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LifeStateChangeHandler() noexcept override;

		LifeStateChangeHandler(const LifeStateChangeHandler&) = delete;
		LifeStateChangeHandler(LifeStateChangeHandler&&) = delete;

		LifeStateChangeHandler& operator=(const LifeStateChangeHandler&) = delete;
		LifeStateChangeHandler& operator=(LifeStateChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetOldStateFilter() const noexcept { return _oldStateFilter; }
		[[nodiscard]] const auto& GetNewStateFilter() const noexcept { return _newStateFilter; }

		RE::BSEventNotifyControl ProcessEvent(const LifeStateChangeEvent& a_event,
			RE::BSTEventSource<LifeStateChangeEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptValueFilter<RE::ACTOR_LIFE_STATE> _oldStateFilter;
		Script::ScriptValueFilter<RE::ACTOR_LIFE_STATE> _newStateFilter;
	};
}
