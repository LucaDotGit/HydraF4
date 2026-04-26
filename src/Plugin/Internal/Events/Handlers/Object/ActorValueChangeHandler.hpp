#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/ActorValueChangeSource.hpp"

namespace Plugin::Internal::Events
{
	class ActorValueChangeHandler final
		: public EventHandler<
			  ActorValueChangeHandler,
			  ActorValueChangeArgs,
			  ActorValueChangeParams,
			  "OnActorValueChange">,
		  public RE::BSTEventSink<ActorValueChangeEvent>
	{
	public:
		ActorValueChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ActorValueChangeHandler() noexcept override;

		ActorValueChangeHandler(const ActorValueChangeHandler&) = delete;
		ActorValueChangeHandler(ActorValueChangeHandler&&) = delete;

		ActorValueChangeHandler& operator=(const ActorValueChangeHandler&) = delete;
		ActorValueChangeHandler& operator=(ActorValueChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetSourceValueFilter() const noexcept { return _sourceValueFilter; }

		RE::BSEventNotifyControl ProcessEvent(const ActorValueChangeEvent& a_event,
			RE::BSTEventSource<ActorValueChangeEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _sourceValueFilter;
	};
}
