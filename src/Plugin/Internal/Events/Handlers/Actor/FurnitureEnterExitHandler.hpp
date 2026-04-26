#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class FurnitureEnterExitHandler final
		: public EventHandler<
			  FurnitureEnterExitHandler,
			  FurnitureEnterExitArgs,
			  FurnitureEnterExitParams,
			  "OnFurnitureEnterExit">,
		  public RE::BSTEventSink<RE::TESFurnitureEvent>
	{
	public:
		FurnitureEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~FurnitureEnterExitHandler() noexcept override;

		FurnitureEnterExitHandler(const FurnitureEnterExitHandler&) = delete;
		FurnitureEnterExitHandler(FurnitureEnterExitHandler&&) = delete;

		FurnitureEnterExitHandler& operator=(const FurnitureEnterExitHandler&) = delete;
		FurnitureEnterExitHandler& operator=(FurnitureEnterExitHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent& a_event,
			RE::BSTEventSource<RE::TESFurnitureEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetRefFilter;
	};
}
