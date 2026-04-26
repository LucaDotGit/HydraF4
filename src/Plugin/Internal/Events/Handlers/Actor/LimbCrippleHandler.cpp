#include "Plugin/Internal/Events/Handlers/Actor/LimbCrippleHandler.hpp"

namespace Plugin::Internal::Events
{
	LimbCrippleHandler::LimbCrippleHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceActorFilter(a_args, "kIncludedSourceActors"sv, "kExcludedSourceActors"sv),
		  _sourceLimbFilter(a_args, "kIncludedSourceLimbs"sv, "kExcludedSourceLimbs"sv)
	{
		RegisterEvents();
	}

	LimbCrippleHandler::~LimbCrippleHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl LimbCrippleHandler::ProcessEvent(const RE::TESLimbCrippleEvent& a_event,
		RE::BSTEventSource<RE::TESLimbCrippleEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto sourceActor = RE::dynamic_ni_pointer_cast<RE::Actor>(a_event.targetRef);
		if (!sourceActor) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceLimb = a_event.limbInfo;
		if (!sourceLimb) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceActorFilter.IsMatch(sourceActor.get()) ||
			!_sourceLimbFilter.IsMatch(sourceLimb)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceActor"sv, sourceActor);
		params.Insert("kSourceLimb"sv, sourceLimb);
		params.Insert("bCrippled"sv, a_event.crippled);
		params.Insert("bPartialCrippled"sv, a_event.partial);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void LimbCrippleHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESLimbCrippleEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void LimbCrippleHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESLimbCrippleEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
