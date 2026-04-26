#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectLoadUnloadHandler final
		: public EventHandler<
			  ObjectLoadUnloadHandler,
			  ObjectLoadUnloadArgs,
			  ObjectLoadUnloadParams,
			  "OnObjectLoadUnload">,
		  public RE::BSTEventSink<RE::TESObjectLoadedEvent>
	{
	public:
		ObjectLoadUnloadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectLoadUnloadHandler() noexcept override;

		ObjectLoadUnloadHandler(const ObjectLoadUnloadHandler&) = delete;
		ObjectLoadUnloadHandler(ObjectLoadUnloadHandler&&) = delete;

		ObjectLoadUnloadHandler& operator=(const ObjectLoadUnloadHandler&) = delete;
		ObjectLoadUnloadHandler& operator=(ObjectLoadUnloadHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESObjectLoadedEvent& a_event,
			RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
	};
}
