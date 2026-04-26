#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class SceneStartStopHandler final
		: public EventHandler<
			  SceneStartStopHandler,
			  SceneStartStopArgs,
			  SceneStartStopParams,
			  "OnSceneStartStop">,
		  public RE::BSTEventSink<RE::TESSceneEvent>
	{
	public:
		SceneStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~SceneStartStopHandler() noexcept override;

		SceneStartStopHandler(const SceneStartStopHandler&) = delete;
		SceneStartStopHandler(SceneStartStopHandler&&) = delete;

		SceneStartStopHandler& operator=(const SceneStartStopHandler&) = delete;
		SceneStartStopHandler& operator=(SceneStartStopHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceSceneFilter() const noexcept { return _sourceSceneFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSceneEvent& a_event,
			RE::BSTEventSource<RE::TESSceneEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceSceneFilter;
	};
}
