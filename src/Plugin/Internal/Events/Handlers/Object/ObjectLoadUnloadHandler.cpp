#include "Plugin/Internal/Events/Handlers/Object/ObjectLoadUnloadHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectLoadUnloadHandler::ObjectLoadUnloadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv)
	{
		RegisterEvents();
	}

	ObjectLoadUnloadHandler::~ObjectLoadUnloadHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectLoadUnloadHandler::ProcessEvent(const RE::TESObjectLoadedEvent& a_event,
		RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = RE::TESForm::FindFormByNumericID<RE::TESObjectREFR>(a_event.formID);
		if (!sourceRef) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("bLoaded"sv, a_event.loaded);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectLoadUnloadHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESObjectLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectLoadUnloadHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESObjectLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
