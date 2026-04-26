#include "Plugin/Internal/Events/Handlers/Object/ObjectGrabReleaseHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectGrabReleaseHandler::ObjectGrabReleaseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	ObjectGrabReleaseHandler::~ObjectGrabReleaseHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectGrabReleaseHandler::ProcessEvent(const RE::TESGrabReleaseEvent& a_event,
		RE::BSTEventSource<RE::TESGrabReleaseEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.targetRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("bGrabbed"sv, a_event.grabbed);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectGrabReleaseHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESGrabReleaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectGrabReleaseHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESGrabReleaseEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
