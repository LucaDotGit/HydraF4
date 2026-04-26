#include "Plugin/Internal/Events/Handlers/Actor/ActiveEffectApplyRemoveHandler.hpp"

namespace Plugin::Internal::Events
{
	ActiveEffectApplyRemoveHandler::ActiveEffectApplyRemoveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _targetActorFilter(a_args, "kIncludedTargetActors"sv, "kExcludedTargetActors"sv),
		  _baseEffectFilter(a_args, "kIncludedBaseEffects"sv, "kExcludedBaseEffects"sv)
	{
		RegisterEvents();
	}

	ActiveEffectApplyRemoveHandler::~ActiveEffectApplyRemoveHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ActiveEffectApplyRemoveHandler::ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent& a_event,
		RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto targetActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.targetRef);
		if (!targetActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto activeEffect = targetActor->GetActiveEffectByID(a_event.activeEffectUniqueID);
		if (!activeEffect) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* baseEffect = activeEffect->spell;
		if (!baseEffect) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.casterRef);

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_targetActorFilter.IsMatch(targetActor.get()) ||
			!_baseEffectFilter.IsMatch(baseEffect)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetActor"sv, targetActor);
		params.Insert("kBaseEffect"sv, baseEffect);
		params.Insert("kActiveEffect"sv, activeEffect);
		params.Insert("bApplied"sv, a_event.applied);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ActiveEffectApplyRemoveHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESActiveEffectApplyRemoveEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ActiveEffectApplyRemoveHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESActiveEffectApplyRemoveEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
