#include "Plugin/Internal/Events/Handlers/Actor/FurnitureEnterExitHandler.hpp"

namespace Plugin::Internal::Events
{
	FurnitureEnterExitHandler::FurnitureEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	FurnitureEnterExitHandler::~FurnitureEnterExitHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl FurnitureEnterExitHandler::ProcessEvent(const RE::TESFurnitureEvent& a_event,
		RE::BSTEventSource<RE::TESFurnitureEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.actorRef);
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetFurnitureRef = a_event.furnitureRef.get();
		if (!targetFurnitureRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_targetRefFilter.IsMatch(targetFurnitureRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto isEntered = a_event.eventType == RE::TESFurnitureEvent::EventType::kEnter;

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetRef"sv, targetFurnitureRef);
		params.Insert("bEntered"sv, isEntered);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void FurnitureEnterExitHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESFurnitureEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void FurnitureEnterExitHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESFurnitureEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
