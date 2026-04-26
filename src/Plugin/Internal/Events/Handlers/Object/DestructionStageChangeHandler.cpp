#include "Plugin/Internal/Events/Handlers/Object/DestructionStageChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	DestructionStageChangeHandler::DestructionStageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _oldStageFilter(a_args, "kIncludedOldStages"sv, "kExcludedOldStages"sv),
		  _newStageFilter(a_args, "kIncludedNewStages"sv, "kExcludedNewStages"sv)
	{
		RegisterEvents();
	}

	DestructionStageChangeHandler::~DestructionStageChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl DestructionStageChangeHandler::ProcessEvent(const RE::TESDestructionStageChangedEvent& a_event,
		RE::BSTEventSource<RE::TESDestructionStageChangedEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.ref.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_oldStageFilter.IsMatch(a_event.oldStage) ||
			!_newStageFilter.IsMatch(a_event.newStage)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("iOldStage"sv, a_event.oldStage);
		params.Insert("iNewStage"sv, a_event.newStage);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void DestructionStageChangeHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESDestructionStageChangedEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void DestructionStageChangeHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESDestructionStageChangedEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
