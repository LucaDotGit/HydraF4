#include "Plugin/Internal/Events/Handlers/Object/ObjectOpenCloseHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectOpenCloseHandler::ObjectOpenCloseHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	ObjectOpenCloseHandler::~ObjectOpenCloseHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectOpenCloseHandler::ProcessEvent(const RE::TESOpenCloseEvent& a_event,
		RE::BSTEventSource<RE::TESOpenCloseEvent>* a_eventSource)
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

		const auto* targetRef = a_event.targetRef.get();

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("bOpened"sv, a_event.opened);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectOpenCloseHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESOpenCloseEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectOpenCloseHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESOpenCloseEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
