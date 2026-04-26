#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ItemEquipUnequipHandler final
		: public EventHandler<
			  ItemEquipUnequipHandler,
			  ItemEquipUnequipArgs,
			  ItemEquipUnequipParams,
			  "OnItemEquipUnequip">,
		  public RE::BSTEventSink<RE::TESEquipEvent>
	{
	public:
		ItemEquipUnequipHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ItemEquipUnequipHandler() noexcept override;

		ItemEquipUnequipHandler(const ItemEquipUnequipHandler&) = delete;
		ItemEquipUnequipHandler(ItemEquipUnequipHandler&&) = delete;

		ItemEquipUnequipHandler& operator=(const ItemEquipUnequipHandler&) = delete;
		ItemEquipUnequipHandler& operator=(ItemEquipUnequipHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetActorFilter() const noexcept { return _targetActorFilter; }
		[[nodiscard]] const auto& GetItemFilter() const noexcept { return _itemFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent& a_event,
			RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetActorFilter;
		Script::ScriptVariantFilter _itemFilter;
	};
}
