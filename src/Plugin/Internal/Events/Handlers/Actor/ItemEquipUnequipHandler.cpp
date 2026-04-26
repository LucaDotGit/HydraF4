#include "Plugin/Internal/Events/Handlers/Actor/ItemEquipUnequipHandler.hpp"

namespace Plugin::Internal::Events
{
	ItemEquipUnequipHandler::ItemEquipUnequipHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetActorFilter(a_args, "kIncludedTargetActors"sv, "kExcludedTargetActors"sv),
		  _itemFilter(a_args, "kIncludedItems"sv, "kExcludedItems"sv)
	{
		RegisterEvents();
	}

	ItemEquipUnequipHandler::~ItemEquipUnequipHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ItemEquipUnequipHandler::ProcessEvent(const RE::TESEquipEvent& a_event,
		RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.actorRef);
		if (!targetActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* item = RE::TESForm::FindFormByNumericID(a_event.baseItemFormID);
		if (!item) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* itemRef = RE::TESForm::FindFormByNumericID<RE::TESObjectREFR>(a_event.itemRefFormID);

		if (!_targetActorFilter.IsMatch(targetActor.get()) ||
			!(itemRef ? _itemFilter.IsMatch(itemRef) : _itemFilter.IsMatch(item))) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kTargetActor"sv, targetActor);
		params.Insert("kItemRef"sv, itemRef);
		params.Insert("kItem"sv, item);
		params.Insert("bEquipped"sv, a_event.equipped);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ItemEquipUnequipHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESEquipEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ItemEquipUnequipHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESEquipEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
