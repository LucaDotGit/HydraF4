#include "Plugin/Internal/Events/Handlers/Object/ObjectSellHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectSellHandler::ObjectSellHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetActorFilter(a_args, "kIncludedTargetActors"sv, "kExcludedTargetActors"sv),
		  _itemRefFilter(a_args, "kIncludedItemRefs"sv, "kExcludedItemRefs"sv)
	{
		RegisterEvents();
	}

	ObjectSellHandler::~ObjectSellHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectSellHandler::ProcessEvent(const RE::TESSellEvent& a_event,
		RE::BSTEventSource<RE::TESSellEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = RE::TESForm::FindFormByNumericID<RE::Actor>(a_event.buyerFormID);
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActorRef = a_event.sellerActorRef;
		if (!targetActorRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActor = RE::dynamic_ni_pointer_cast<RE::Actor>(targetActorRef);
		if (!targetActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto itemRef = RE::GameScript::GetRefInventoryObjectRef(targetActorRef, a_event.uniqueID);
		if (!itemRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetActorFilter.IsMatch(targetActor.get()) ||
			!_itemRefFilter.IsMatch(itemRef.get())) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetActor"sv, targetActor);
		params.Insert("kItemRef"sv, itemRef);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectSellHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESSellEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectSellHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESSellEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
