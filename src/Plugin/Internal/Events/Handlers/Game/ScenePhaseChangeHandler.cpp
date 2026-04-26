#include "Plugin/Internal/Events/Handlers/Game/ScenePhaseChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	ScenePhaseChangeHandler::ScenePhaseChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceSceneFilter(a_args, "kIncludedSourceScenes"sv, "kExcludedSourceScenes"sv),
		  _newPhaseIndexFilter(a_args, "kIncludedNewPhaseIndexes"sv, "kExcludedNewPhaseIndexes"sv)
	{
		RegisterEvents();
	}

	ScenePhaseChangeHandler::~ScenePhaseChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ScenePhaseChangeHandler::ProcessEvent(const RE::TESScenePhaseEvent& a_event,
		RE::BSTEventSource<RE::TESScenePhaseEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceScene = RE::TESForm::FindFormByNumericID<RE::BGSScene>(a_event.sceneFormID);
		if (!sourceScene) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto newPhaseIndex = a_event.phaseIndex;

		if (!_sourceSceneFilter.IsMatch(sourceScene) ||
			!_newPhaseIndexFilter.IsMatch(newPhaseIndex)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceScene"sv, sourceScene);
		params.Insert("iNewPhaseIndex"sv, newPhaseIndex);
		params.Insert("bStarted"sv, a_event.eventType == RE::TESScenePhaseEvent::EventType::kBegin);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ScenePhaseChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESScenePhaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ScenePhaseChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESScenePhaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
