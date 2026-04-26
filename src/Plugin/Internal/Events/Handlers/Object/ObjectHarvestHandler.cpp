#include "Plugin/Internal/Events/Handlers/Object/ObjectHarvestHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectHarvestHandler::ObjectHarvestHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv),
		  _itemFilter(a_args, "kIncludedItems"sv, "kExcludedItems"sv)
	{
		RegisterEvents();
	}

	ObjectHarvestHandler::~ObjectHarvestHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectHarvestHandler::ProcessEvent(const RE::TESHarvestEvent::ItemHarvested& a_event,
		RE::BSTEventSource<RE::TESHarvestEvent::ItemHarvested>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceActor = a_event.harvesterActor;
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.harvestedRef;
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* item = a_event.harvestedItem;
		if (!item) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceActorFilter.IsMatch(sourceActor) ||
			!_targetRefFilter.IsMatch(targetRef) ||
			!_itemFilter.IsMatch(item)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("kItem"sv, item);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectHarvestHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESHarvestEvent::ItemHarvested::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectHarvestHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESHarvestEvent::ItemHarvested::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
