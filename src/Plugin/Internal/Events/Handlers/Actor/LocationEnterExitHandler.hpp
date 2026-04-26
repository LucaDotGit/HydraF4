#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class LocationEnterExitHandler final
		: public EventHandler<
			  LocationEnterExitHandler,
			  LocationEnterExitArgs,
			  LocationEnterExitParams,
			  "OnLocationEnterExit">,
		  public RE::BSTEventSink<RE::TESActorLocationChangeEvent>
	{
	public:
		LocationEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~LocationEnterExitHandler() noexcept override;

		LocationEnterExitHandler(const LocationEnterExitHandler&) = delete;
		LocationEnterExitHandler(LocationEnterExitHandler&&) = delete;

		LocationEnterExitHandler& operator=(const LocationEnterExitHandler&) = delete;
		LocationEnterExitHandler& operator=(LocationEnterExitHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetOldLocationFilter() const noexcept { return _oldLocationFilter; }
		[[nodiscard]] const auto& GetNewLocationFilter() const noexcept { return _newLocationFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESActorLocationChangeEvent& a_event,
			RE::BSTEventSource<RE::TESActorLocationChangeEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _oldLocationFilter;
		Script::ScriptVariantFilter _newLocationFilter;
	};
}
