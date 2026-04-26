#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectSellHandler final
		: public EventHandler<
			  ObjectSellHandler,
			  ObjectSellArgs,
			  ObjectSellParams,
			  "OnObjectSell">,
		  public RE::BSTEventSink<RE::TESSellEvent>
	{
	public:
		ObjectSellHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectSellHandler() noexcept override;

		ObjectSellHandler(const ObjectSellHandler&) = delete;
		ObjectSellHandler(ObjectSellHandler&&) = delete;

		ObjectSellHandler& operator=(const ObjectSellHandler&) = delete;
		ObjectSellHandler& operator=(ObjectSellHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetActorFilter() const noexcept { return _targetActorFilter; }
		[[nodiscard]] const auto& GetItemRefFilter() const noexcept { return _itemRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSellEvent& a_event,
			RE::BSTEventSource<RE::TESSellEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetActorFilter;
		Script::ScriptVariantFilter _itemRefFilter;
	};
}
