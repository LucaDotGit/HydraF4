#include "Plugin/Internal/Events/Handlers/Game/LockPickHandler.hpp"

namespace Plugin::Internal::Events
{
	LockPickHandler::LockPickHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	LockPickHandler::~LockPickHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LockPickHandler::ProcessEvent([[maybe_unused]] const RE::LocksPicked::Event& a_event,
		RE::BSTEventSource<RE::LocksPicked::Event>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = RE::LockpickingMenu::GetCurrentLockRef();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_targetRefFilter.IsMatch(targetRef)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto enumLockLevel = RE::LockpickingMenu::GetCurrentLockLevel();
		const auto numericLockLevel = RE::REFR_LOCK::GetNumericLockLevel(enumLockLevel);

		auto params = EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("iLockLevel"sv, numericLockLevel);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LockPickHandler::RegisterEvents()
	{
		RE::LocksPicked::Event::GetEventSource()->RegisterSink(this);
	}

	void LockPickHandler::UnregisterEvents()
	{
		RE::LocksPicked::Event::GetEventSource()->UnregisterSink(this);
	}
}
