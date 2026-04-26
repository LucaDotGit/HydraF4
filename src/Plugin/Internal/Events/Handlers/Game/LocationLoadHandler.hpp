#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class LocationLoadHandler final
		: public EventHandler<
			  LocationLoadHandler,
			  LocationLoadArgs,
			  LocationLoadParams,
			  "OnLocationLoad">,
		  public RE::BSTEventSink<RE::BGSLocationLoadedEvent>
	{
	public:
		LocationLoadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LocationLoadHandler() noexcept override;

		LocationLoadHandler(const LocationLoadHandler&) = delete;
		LocationLoadHandler(LocationLoadHandler&&) = delete;

		LocationLoadHandler& operator=(const LocationLoadHandler&) = delete;
		LocationLoadHandler& operator=(LocationLoadHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceLocationFilter() const noexcept { return _sourceLocationFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::BGSLocationLoadedEvent& a_event,
			RE::BSTEventSource<RE::BGSLocationLoadedEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceLocationFilter;
	};
}
