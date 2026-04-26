#include "Plugin/Internal/Events/Handlers/Game/SceneStartStopHandler.hpp"

namespace Plugin::Internal::Events
{
	SceneStartStopHandler::SceneStartStopHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceSceneFilter(a_args, "kIncludedSourceScenes"sv, "kExcludedSourceScenes"sv)
	{
		RegisterEvents();
	}

	SceneStartStopHandler::~SceneStartStopHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl SceneStartStopHandler::ProcessEvent(const RE::TESSceneEvent& a_event,
		RE::BSTEventSource<RE::TESSceneEvent>* a_eventSource)
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

		if (!_sourceSceneFilter.IsMatch(sourceScene)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceScene"sv, sourceScene);
		params.Insert("bStarted"sv, a_event.eventType == RE::TESSceneEvent::EventType::kBegin);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void SceneStartStopHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESSceneEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void SceneStartStopHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESSceneEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
