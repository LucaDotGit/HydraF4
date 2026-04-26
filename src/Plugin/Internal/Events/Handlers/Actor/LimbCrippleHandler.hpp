#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class LimbCrippleHandler final
		: public EventHandler<
			  LimbCrippleHandler,
			  LimbCrippleArgs,
			  LimbCrippleParams,
			  "OnLimbCripple">,
		  public RE::BSTEventSink<RE::TESLimbCrippleEvent>
	{
	public:
		LimbCrippleHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LimbCrippleHandler() noexcept override;

		LimbCrippleHandler(const LimbCrippleHandler&) = delete;
		LimbCrippleHandler(LimbCrippleHandler&&) = delete;

		LimbCrippleHandler& operator=(const LimbCrippleHandler&) = delete;
		LimbCrippleHandler& operator=(LimbCrippleHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetSourceLimbFilter() const noexcept { return _sourceLimbFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESLimbCrippleEvent& a_event,
			RE::BSTEventSource<RE::TESLimbCrippleEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _sourceLimbFilter;
	};
}
