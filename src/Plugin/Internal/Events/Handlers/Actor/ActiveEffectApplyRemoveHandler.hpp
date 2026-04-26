#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ActiveEffectApplyRemoveHandler final
		: public EventHandler<
			  ActiveEffectApplyRemoveHandler,
			  ActiveEffectApplyRemoveArgs,
			  ActiveEffectApplyRemoveParams,
			  "OnActiveEffectApplyRemove">,
		  public RE::BSTEventSink<RE::TESActiveEffectApplyRemoveEvent>
	{
	public:
		ActiveEffectApplyRemoveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ActiveEffectApplyRemoveHandler() noexcept override;

		ActiveEffectApplyRemoveHandler(const ActiveEffectApplyRemoveHandler&) = delete;
		ActiveEffectApplyRemoveHandler(ActiveEffectApplyRemoveHandler&&) = delete;

		ActiveEffectApplyRemoveHandler& operator=(const ActiveEffectApplyRemoveHandler&) = delete;
		ActiveEffectApplyRemoveHandler& operator=(ActiveEffectApplyRemoveHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetActorFilter() const noexcept { return _targetActorFilter; }
		[[nodiscard]] const auto& GetBaseEffectFilter() const noexcept { return _baseEffectFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent& a_event,
			RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetActorFilter;
		Script::ScriptVariantFilter _baseEffectFilter;
	};
}
