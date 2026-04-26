#include "Plugin/Internal/Events/Handlers/Game/CrosshairRefChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	CrosshairRefChangeHandler::CrosshairRefChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv)
	{
		RegisterEvents();
	}

	CrosshairRefChangeHandler::~CrosshairRefChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CrosshairRefChangeHandler::ProcessEvent(const CrosshairRefChangeEvent& a_event,
		RE::BSTEventSource<CrosshairRefChangeEvent>* a_eventSource)
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

		auto params = CrosshairRefChangeHandler::EventParams::Create();
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("bTargeted"sv, a_event.isTargeted);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void CrosshairRefChangeHandler::RegisterEvents()
	{
		CrosshairRefChangeSource::GetEventSource()->RegisterSink(this);
	}

	void CrosshairRefChangeHandler::UnregisterEvents()
	{
		CrosshairRefChangeSource::GetEventSource()->UnregisterSink(this);
	}
}
