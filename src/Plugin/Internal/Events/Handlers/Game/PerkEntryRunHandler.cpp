#include "Plugin/Internal/Events/Handlers/Game/PerkEntryRunHandler.hpp"

namespace Plugin::Internal::Events
{
	PerkEntryRunHandler::PerkEntryRunHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv),
		  _perkFilter(a_args, "kIncludedPerks"sv, "kExcludedPerks"sv),
		  _entryIdFilter(a_args, "kIncludedEntryIds"sv, "kExcludedEntryIds"sv)
	{
		RegisterEvents();
	}

	PerkEntryRunHandler::~PerkEntryRunHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl PerkEntryRunHandler::ProcessEvent(const RE::TESPerkEntryRunEvent& a_event,
		RE::BSTEventSource<RE::TESPerkEntryRunEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceActor = a_event.perkOwnerRef.get();
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* perk = RE::TESForm::FindFormByNumericID<RE::BGSPerk>(a_event.perkFormID);
		if (!perk) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.targetRef.get();

		const auto entryId = a_event.perkEntryID;

		if (!_sourceActorFilter.IsMatch(sourceActor) ||
			!_targetRefFilter.IsMatch(targetRef) ||
			!_perkFilter.IsMatch(perk) ||
			!_entryIdFilter.IsMatch(entryId)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("kPerk"sv, perk);
		params.Insert("iEntryId"sv, entryId);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void PerkEntryRunHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESPerkEntryRunEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void PerkEntryRunHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESPerkEntryRunEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
