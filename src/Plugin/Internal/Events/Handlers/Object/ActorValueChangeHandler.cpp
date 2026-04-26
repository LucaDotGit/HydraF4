#include "Plugin/Internal/Events/Handlers/Object/ActorValueChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	ActorValueChangeHandler::ActorValueChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _sourceValueFilter(a_args, "kIncludedSourceValues"sv, "kExcludedSourceValues"sv)
	{
		RegisterEvents();
	}

	ActorValueChangeHandler::~ActorValueChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ActorValueChangeHandler::ProcessEvent(const ActorValueChangeEvent& a_event,
		RE::BSTEventSource<ActorValueChangeEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.sourceRef.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceValue = a_event.sourceValue;
		if (!sourceValue) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_sourceValueFilter.IsMatch(sourceValue)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kSourceValue"sv, sourceValue);
		params.Insert("fOldValue"sv, a_event.oldValue);
		params.Insert("fNewValue"sv, a_event.newValue);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ActorValueChangeHandler::RegisterEvents()
	{
		ActorValueChangeSource::GetEventSource()->RegisterSink(this);
	}

	void ActorValueChangeHandler::UnregisterEvents()
	{
		ActorValueChangeSource::GetEventSource()->UnregisterSink(this);
	}
}
