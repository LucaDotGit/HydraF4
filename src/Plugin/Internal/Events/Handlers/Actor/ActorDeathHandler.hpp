#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ActorDeathHandler final
		: public EventHandler<
			  ActorDeathHandler,
			  ActorDeathArgs,
			  ActorDeathParams,
			  "OnActorDeath">,
		  public RE::BSTEventSink<RE::TESDeathEvent>
	{
	public:
		ActorDeathHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ActorDeathHandler() noexcept override;

		ActorDeathHandler(const ActorDeathHandler&) = delete;
		ActorDeathHandler(ActorDeathHandler&&) = delete;

		ActorDeathHandler& operator=(const ActorDeathHandler&) = delete;
		ActorDeathHandler& operator=(ActorDeathHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetActorFilter() const noexcept { return _targetActorFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent& a_event,
			RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetActorFilter;
	};
}
