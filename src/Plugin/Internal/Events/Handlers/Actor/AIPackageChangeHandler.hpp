#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class AIPackageChangeHandler final
		: public EventHandler<
			  AIPackageChangeHandler,
			  AIPackageChangeArgs,
			  AIPackageChangeParams,
			  "OnAIPackageChange">,
		  public RE::BSTEventSink<RE::TESPackageEvent>
	{
	public:
		AIPackageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~AIPackageChangeHandler() noexcept override;

		AIPackageChangeHandler(const AIPackageChangeHandler&) = delete;
		AIPackageChangeHandler(AIPackageChangeHandler&&) = delete;

		AIPackageChangeHandler& operator=(const AIPackageChangeHandler&) = delete;
		AIPackageChangeHandler& operator=(AIPackageChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetNewAIPackageFilter() const noexcept { return _newAIPackageFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESPackageEvent& a_event,
			RE::BSTEventSource<RE::TESPackageEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _newAIPackageFilter;
	};
}
