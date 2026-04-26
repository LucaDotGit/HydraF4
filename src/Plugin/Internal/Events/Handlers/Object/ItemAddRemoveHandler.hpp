#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ItemAddRemoveHandler final
		: public EventHandler<
			  ItemAddRemoveHandler,
			  ItemAddRemoveArgs,
			  ItemAddRemoveParams,
			  "OnItemAddRemove">,
		  public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		ItemAddRemoveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ItemAddRemoveHandler() noexcept override;

		ItemAddRemoveHandler(const ItemAddRemoveHandler&) = delete;
		ItemAddRemoveHandler(ItemAddRemoveHandler&&) = delete;

		ItemAddRemoveHandler& operator=(const ItemAddRemoveHandler&) = delete;
		ItemAddRemoveHandler& operator=(ItemAddRemoveHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }
		[[nodiscard]] const auto& GetItemFilter() const noexcept { return _itemFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent& a_event,
			RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetRefFilter;
		Script::ScriptVariantFilter _itemFilter;
	};
}
