#include "Plugin/Internal/Events/Handlers/Actor/AIPackageChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	AIPackageChangeHandler::AIPackageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _newAIPackageFilter(a_args, "kIncludedNewAIPackages"sv, "kExcludedNewAIPackages"sv)
	{
		RegisterEvents();
	}

	AIPackageChangeHandler::~AIPackageChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl AIPackageChangeHandler::ProcessEvent(const RE::TESPackageEvent& a_event,
		RE::BSTEventSource<RE::TESPackageEvent>* a_eventSource)
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

		const auto* newAIPackage = RE::TESForm::FindFormByNumericID<RE::TESPackage>(a_event.packageFormID);
		if (!newAIPackage) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_newAIPackageFilter.IsMatch(newAIPackage)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kNewAIPackage"sv, newAIPackage);
		params.Insert("bStarted"sv, a_event.eventType == RE::TESPackageEvent::EventType::kBegin);
		params.Insert("bChanged"sv, a_event.eventType == RE::TESPackageEvent::EventType::kChange);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void AIPackageChangeHandler::RegisterEvents()
	{
		RE::TESPackageEvent::GetEventSource()->RegisterSink(this);
	}

	void AIPackageChangeHandler::UnregisterEvents()
	{
		RE::TESPackageEvent::GetEventSource()->UnregisterSink(this);
	}
}
