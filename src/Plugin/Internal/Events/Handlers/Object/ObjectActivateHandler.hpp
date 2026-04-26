#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectActivateHandler final
		: public EventHandler<
			  ObjectActivateHandler,
			  ObjectActivateArgs,
			  ObjectActivateParams,
			  "OnObjectActivate">,
		  public RE::BSTEventSink<RE::TESActivateEvent>
	{
	public:
		ObjectActivateHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectActivateHandler() noexcept override;

		ObjectActivateHandler(const ObjectActivateHandler&) = delete;
		ObjectActivateHandler(ObjectActivateHandler&&) = delete;

		ObjectActivateHandler& operator=(const ObjectActivateHandler&) = delete;
		ObjectActivateHandler& operator=(ObjectActivateHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent& a_event,
			RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetRefFilter;
	};
}
