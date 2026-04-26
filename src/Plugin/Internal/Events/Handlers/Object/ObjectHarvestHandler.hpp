#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectHarvestHandler final
		: public EventHandler<
			  ObjectHarvestHandler,
			  ObjectHarvestArgs,
			  ObjectHarvestParams,
			  "OnObjectHarvest">,
		  public RE::BSTEventSink<RE::TESHarvestEvent::ItemHarvested>
	{
	public:
		ObjectHarvestHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectHarvestHandler() noexcept override;

		ObjectHarvestHandler(const ObjectHarvestHandler&) = delete;
		ObjectHarvestHandler(ObjectHarvestHandler&&) = delete;

		ObjectHarvestHandler& operator=(const ObjectHarvestHandler&) = delete;
		ObjectHarvestHandler& operator=(ObjectHarvestHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }
		[[nodiscard]] const auto& GetItemFilter() const noexcept { return _itemFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESHarvestEvent::ItemHarvested& a_event,
			RE::BSTEventSource<RE::TESHarvestEvent::ItemHarvested>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetRefFilter;
		Script::ScriptVariantFilter _itemFilter;
	};
}
