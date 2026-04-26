#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ScenePhaseChangeHandler final
		: public EventHandler<
			  ScenePhaseChangeHandler,
			  ScenePhaseChangeArgs,
			  ScenePhaseChangeParams,
			  "OnScenePhaseChange">,
		  public RE::BSTEventSink<RE::TESScenePhaseEvent>
	{
	public:
		ScenePhaseChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ScenePhaseChangeHandler() noexcept override;

		ScenePhaseChangeHandler(const ScenePhaseChangeHandler&) = delete;
		ScenePhaseChangeHandler(ScenePhaseChangeHandler&&) = delete;

		ScenePhaseChangeHandler& operator=(const ScenePhaseChangeHandler&) = delete;
		ScenePhaseChangeHandler& operator=(ScenePhaseChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceSceneFilter() const noexcept { return _sourceSceneFilter; }
		[[nodiscard]] const auto& GetNewPhaseIndexFilter() const noexcept { return _newPhaseIndexFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESScenePhaseEvent& a_event,
			RE::BSTEventSource<RE::TESScenePhaseEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceSceneFilter;
		Script::ScriptValueFilter<std::uint32_t> _newPhaseIndexFilter;
	};
}
