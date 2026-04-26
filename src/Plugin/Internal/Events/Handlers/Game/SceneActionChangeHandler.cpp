#include "Plugin/Internal/Events/Handlers/Game/SceneActionChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	SceneActionChangeHandler::SceneActionChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceSceneFilter(a_args, "kIncludedSourceScenes"sv, "kExcludedSourceScenes"sv),
		  _newActionIdFilter(a_args, "kIncludedNewActionIds"sv, "kExcludedNewActionIds"sv)
	{
		RegisterEvents();
	}

	SceneActionChangeHandler::~SceneActionChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl SceneActionChangeHandler::ProcessEvent(const RE::TESSceneActionEvent& a_event,
		RE::BSTEventSource<RE::TESSceneActionEvent>* a_eventSource)
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

		const auto* sourceQuest = RE::TESForm::FindFormByNumericID<RE::TESQuest>(a_event.questFormID);
		if (!sourceQuest) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto newActionId = a_event.actionID;

		const auto* baseAlias = sourceQuest->GetAliasByID(a_event.refAliasID);

		if (!_sourceSceneFilter.IsMatch(sourceScene) ||
			!_newActionIdFilter.IsMatch(newActionId)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceScene"sv, sourceScene);
		params.Insert("iNewActionId"sv, newActionId);
		params.Insert("kRefAlias"sv, baseAlias ? baseAlias->As<RE::BGSRefAlias>() : nullptr);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void SceneActionChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESSceneActionEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void SceneActionChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESSceneActionEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
