#include "Plugin/Internal/Events/Handlers/Game/CellLoadHandler.hpp"

namespace Plugin::Internal::Events
{
	CellLoadHandler::CellLoadHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceCellFilter(a_args, "kIncludedSourceCells"sv, "kExcludedSourceCells"sv)
	{
		RegisterEvents();
	}

	CellLoadHandler::~CellLoadHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CellLoadHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent& a_event,
		RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* a_eventSource)
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

		auto params = EventParams::Create();
		params.Insert("kSourceCell"sv, sourceCell);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void CellLoadHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESCellFullyLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void CellLoadHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESCellFullyLoadedEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
