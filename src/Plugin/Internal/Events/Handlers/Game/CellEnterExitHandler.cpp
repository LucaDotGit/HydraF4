#include "Plugin/Internal/Events/Handlers/Game/CellEnterExitHandler.hpp"

namespace Plugin::Internal::Events
{
	CellEnterExitHandler::CellEnterExitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetCellFilter(a_args, "kIncludedTargetCells"sv, "kExcludedTargetCells"sv)
	{
		RegisterEvents();
	}

	CellEnterExitHandler::~CellEnterExitHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CellEnterExitHandler::ProcessEvent(const RE::BGSActorCellEvent& a_event,
		RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceActor = a_event.actor.underlying();
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetCell = RE::TESForm::FindFormByNumericID<RE::TESObjectCELL>(a_event.cellFormID);
		if (!targetCell) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetCellFilter.IsMatch(targetCell)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto isEntered = a_event.eventType.any(RE::BGSActorCellEvent::EventType::kEnter);

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kTargetCell"sv, targetCell);
		params.Insert("bEntered"sv, isEntered);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void CellEnterExitHandler::RegisterEvents()
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::BGSActorCellEvent>*>(player);
		if (!eventSource) {
			return;
		}

		eventSource->RegisterSink(this);
	}

	void CellEnterExitHandler::UnregisterEvents()
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::BGSActorCellEvent>*>(player);
		if (!eventSource) {
			return;
		}

		eventSource->UnregisterSink(this);
	}
}
