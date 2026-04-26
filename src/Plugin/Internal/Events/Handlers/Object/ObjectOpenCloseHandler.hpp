#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectOpenCloseHandler final
		: public EventHandler<
			  ObjectOpenCloseHandler,
			  ObjectOpenCloseArgs,
			  ObjectOpenCloseParams,
			  "OnObjectOpenClose">,
		  public RE::BSTEventSink<RE::TESOpenCloseEvent>
	{
	public:
		ObjectOpenCloseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectOpenCloseHandler() noexcept override;

		ObjectOpenCloseHandler(const ObjectOpenCloseHandler&) = delete;
		ObjectOpenCloseHandler(ObjectOpenCloseHandler&&) = delete;

		ObjectOpenCloseHandler& operator=(const ObjectOpenCloseHandler&) = delete;
		ObjectOpenCloseHandler& operator=(ObjectOpenCloseHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESOpenCloseEvent& a_event,
			RE::BSTEventSource<RE::TESOpenCloseEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetRefFilter;
	};
}
