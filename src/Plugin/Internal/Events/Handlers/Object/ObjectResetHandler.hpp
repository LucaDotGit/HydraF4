#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectResetHandler final
		: public EventHandler<
			  ObjectResetHandler,
			  ObjectResetArgs,
			  ObjectResetParams,
			  "OnObjectReset">,
		  public RE::BSTEventSink<RE::TESResetEvent>
	{
	public:
		ObjectResetHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectResetHandler() noexcept override;

		ObjectResetHandler(const ObjectResetHandler&) = delete;
		ObjectResetHandler(ObjectResetHandler&&) = delete;

		ObjectResetHandler& operator=(const ObjectResetHandler&) = delete;
		ObjectResetHandler& operator=(ObjectResetHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESResetEvent& a_event,
			RE::BSTEventSource<RE::TESResetEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
	};
}
