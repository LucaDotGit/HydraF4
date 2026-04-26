#include "Plugin/Internal/Events/Handlers/Object/ObjectResetHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectResetHandler::ObjectResetHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv)
	{
		RegisterEvents();
	}

	ObjectResetHandler::~ObjectResetHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectResetHandler::ProcessEvent(const RE::TESResetEvent& a_event,
		RE::BSTEventSource<RE::TESResetEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.ref.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectResetHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESResetEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectResetHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESResetEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
