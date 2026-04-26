#include "Plugin/Internal/Events/Handlers/Object/ObjectActivateHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectActivateHandler::ObjectActivateHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	ObjectActivateHandler::~ObjectActivateHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectActivateHandler::ProcessEvent(const RE::TESActivateEvent& a_event,
		RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.actionRef.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.activatedRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetRef"sv, targetRef);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectActivateHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESActivateEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectActivateHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESActivateEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
