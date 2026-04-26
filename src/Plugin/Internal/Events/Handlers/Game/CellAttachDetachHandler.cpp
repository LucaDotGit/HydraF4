#include "Plugin/Internal/Events/Handlers/Game/CellAttachDetachHandler.hpp"

namespace Plugin::Internal::Events
{
	CellAttachDetachHandler::CellAttachDetachHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceCellFilter(a_args, "kIncludedSourceCells"sv, "kExcludedSourceCells"sv)
	{
		RegisterEvents();
	}

	CellAttachDetachHandler::~CellAttachDetachHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CellAttachDetachHandler::ProcessEvent(const RE::CellAttachDetachEvent& a_event,
		RE::BSTEventSource<RE::CellAttachDetachEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceCell = a_event.cell;
		if (!sourceCell) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceCellFilter.IsMatch(sourceCell)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		using EventAttachType = RE::CellAttachDetachEvent::EventType;

		const auto isAttached = a_event.eventType.any(EventAttachType::kPreAttach, EventAttachType::kPostAttach);
		const auto isPreProcess = a_event.eventType.any(EventAttachType::kPreAttach, EventAttachType::kPreDetach);

		auto params = EventParams::Create();
		params.Insert("kSourceCell"sv, sourceCell);
		params.Insert("bAttached"sv, isAttached);
		params.Insert("bPreProcessed"sv, isPreProcess);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void CellAttachDetachHandler::RegisterEvents()
	{
		auto* eventSource = RE::CellAttachDetachEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void CellAttachDetachHandler::UnregisterEvents()
	{
		auto* eventSource = RE::CellAttachDetachEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
