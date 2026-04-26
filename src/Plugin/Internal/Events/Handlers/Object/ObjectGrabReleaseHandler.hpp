#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectGrabReleaseHandler final
		: public EventHandler<
			  ObjectGrabReleaseHandler,
			  ObjectGrabReleaseArgs,
			  ObjectGrabReleaseParams,
			  "OnObjectGrabRelease">,
		  public RE::BSTEventSink<RE::TESGrabReleaseEvent>
	{
	public:
		ObjectGrabReleaseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectGrabReleaseHandler() noexcept override;

		ObjectGrabReleaseHandler(const ObjectGrabReleaseHandler&) = delete;
		ObjectGrabReleaseHandler(ObjectGrabReleaseHandler&&) = delete;

		ObjectGrabReleaseHandler& operator=(const ObjectGrabReleaseHandler&) = delete;
		ObjectGrabReleaseHandler& operator=(ObjectGrabReleaseHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESGrabReleaseEvent& a_event,
			RE::BSTEventSource<RE::TESGrabReleaseEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
