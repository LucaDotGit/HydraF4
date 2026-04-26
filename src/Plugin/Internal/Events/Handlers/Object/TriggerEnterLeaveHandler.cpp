#include "Plugin/Internal/Events/Handlers/Object/TriggerEnterLeaveHandler.hpp"

namespace Plugin::Internal::Events
{
	TriggerEnterLeaveHandler::TriggerEnterLeaveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	TriggerEnterLeaveHandler::~TriggerEnterLeaveHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl TriggerEnterLeaveHandler::ProcessEvent(const RE::TESTriggerEnterEvent& a_event,
		RE::BSTEventSource<RE::TESTriggerEnterEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!ProcessEventImpl(a_event.actionRef, a_event.triggerRef, true)) [[unlikely]] {
			REX::Assert(false);
			a_eventSource->UnregisterSink(this);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl TriggerEnterLeaveHandler::ProcessEvent(const RE::TESTriggerLeaveEvent& a_event,
		RE::BSTEventSource<RE::TESTriggerLeaveEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!ProcessEventImpl(a_event.actionRef, a_event.triggerRef, false)) [[unlikely]] {
			REX::Assert(false);
			a_eventSource->UnregisterSink(this);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	void TriggerEnterLeaveHandler::RegisterEvents()
	{
		auto* triggerEnterEventSource = RE::TESTriggerEnterEvent::GetEventSource();
		if (triggerEnterEventSource) {
			triggerEnterEventSource->RegisterSink(this);
		}

		auto* triggerLeaveEventSource = RE::TESTriggerLeaveEvent::GetEventSource();
		if (triggerLeaveEventSource) {
			triggerLeaveEventSource->RegisterSink(this);
		}
	}

	void TriggerEnterLeaveHandler::UnregisterEvents()
	{
		auto* triggerEnterEventSource = RE::TESTriggerEnterEvent::GetEventSource();
		if (triggerEnterEventSource) {
			triggerEnterEventSource->UnregisterSink(this);
		}

		auto* triggerLeaveEventSource = RE::TESTriggerLeaveEvent::GetEventSource();
		if (triggerLeaveEventSource) {
			triggerLeaveEventSource->UnregisterSink(this);
		}
	}

	bool TriggerEnterLeaveHandler::ProcessEventImpl(const RE::NiPointer<RE::TESObjectREFR>& a_sourceRef,
		const RE::NiPointer<RE::TESObjectREFR>& a_targetRef, bool a_isEntered) const
	{

		const auto* sourceRef = a_sourceRef.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return true;
		}

		const auto* targetRef = a_targetRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return true;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetRefFilter.IsMatch(targetRef)) {
			return true;
		}

		auto params = TriggerEnterLeaveHandler::EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("bEntered"sv, a_isEntered);

		return _functionRef.InvokeFunction(std::move(params));
	}
}
