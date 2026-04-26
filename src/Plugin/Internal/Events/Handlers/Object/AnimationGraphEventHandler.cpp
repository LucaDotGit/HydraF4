#include "Plugin/Internal/Events/Handlers/Object/AnimationGraphEventHandler.hpp"

namespace Plugin::Internal::Events
{
	AnimationGraphEventHandler::AnimationGraphEventHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _eventNameFilter(a_args, "kIncludedEventNames"sv, "kExcludedEventNames"sv),
		  _payloadFilter(a_args, "kIncludedPayloads"sv, "kExcludedPayloads"sv)
	{
		RegisterEvents();
	}

	AnimationGraphEventHandler::~AnimationGraphEventHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl AnimationGraphEventHandler::ProcessEvent(const AnimationGraphEvent& a_event,
		RE::BSTEventSource<AnimationGraphEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.sourceRef.get();
		if (!sourceRef) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto& eventName = a_event.eventName;
		const auto& payload = a_event.payload;

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_eventNameFilter.IsMatch(eventName) ||
			!_payloadFilter.IsMatch(payload)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("sEventName"sv, eventName);
		params.Insert("sPayload"sv, payload);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void AnimationGraphEventHandler::RegisterEvents()
	{
		AnimationGraphEventSource::GetEventSource()->RegisterSink(this);
	}

	void AnimationGraphEventHandler::UnregisterEvents()
	{
		AnimationGraphEventSource::GetEventSource()->UnregisterSink(this);
	}
}
