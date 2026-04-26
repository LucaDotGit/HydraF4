#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class SceneActionChangeHandler final
		: public EventHandler<
			  SceneActionChangeHandler,
			  SceneActionChangeArgs,
			  SceneActionChangeParams,
			  "OnSceneActionChange">,
		  public RE::BSTEventSink<RE::TESSceneActionEvent>
	{
	public:
		SceneActionChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~SceneActionChangeHandler() noexcept override;

		SceneActionChangeHandler(const SceneActionChangeHandler&) = delete;
		SceneActionChangeHandler(SceneActionChangeHandler&&) = delete;

		SceneActionChangeHandler& operator=(const SceneActionChangeHandler&) = delete;
		SceneActionChangeHandler& operator=(SceneActionChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceSceneFilter() const noexcept { return _sourceSceneFilter; }
		[[nodiscard]] const auto& GetNewActionIdFilter() const noexcept { return _newActionIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSceneActionEvent& a_event,
			RE::BSTEventSource<RE::TESSceneActionEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceSceneFilter;
		Script::ScriptValueFilter<std::uint32_t> _newActionIdFilter;
	};
}
