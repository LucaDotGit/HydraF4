#include "Plugin/Internal/Events/Handlers/Object/ItemAddRemoveHandler.hpp"

namespace Plugin::Internal::Events
{
	ItemAddRemoveHandler::ItemAddRemoveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv),
		  _itemFilter(a_args, "kIncludedItems"sv, "kExcludedItems"sv)
	{
		RegisterEvents();
	}

	ItemAddRemoveHandler::~ItemAddRemoveHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ItemAddRemoveHandler::ProcessEvent(const RE::TESContainerChangedEvent& a_event,
		RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* item = RE::TESForm::FindFormByNumericID(a_event.baseItemFormID);
		if (!item) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = RE::TESForm::FindFormByNumericID<RE::TESObjectREFR>(a_event.oldContainerFormID);
		const auto* targetRef = RE::TESForm::FindFormByNumericID<RE::TESObjectREFR>(a_event.newContainerFormID);

		const auto* itemRef = RE::TESForm::FindFormByNumericID<RE::TESObjectREFR>(a_event.itemRefFormID);

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetRefFilter.IsMatch(targetRef) ||
			!(itemRef ? _itemFilter.IsMatch(itemRef) : _itemFilter.IsMatch(item))) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("kItemRef"sv, itemRef);
		params.Insert("kItem"sv, item);
		params.Insert("iItemCount"sv, a_event.itemCount);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ItemAddRemoveHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESContainerChangedEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ItemAddRemoveHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESContainerChangedEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
