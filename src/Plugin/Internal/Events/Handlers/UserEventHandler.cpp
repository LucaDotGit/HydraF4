#include "Plugin/Internal/Events/Handlers/UserEventHandler.hpp"

namespace Plugin::Internal::Events
{
	UserEventHandler::UserEventHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _eventNameFilter(a_args, "kIncludedEventNames"sv, "kExcludedEventNames"sv)
	{
		RegisterEvents();
	}

	UserEventHandler::~UserEventHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl UserEventHandler::ProcessEvent(const UserEvent& a_event,
		RE::BSTEventSource<UserEvent>* a_eventSource)
	{
		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto& eventName = a_event.eventName;
		const auto& eventArgs = a_event.args;

		if (!_eventNameFilter.IsMatch(eventName)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_event.isApplied) {
			auto clonedEventArgs = eventArgs.Clone();

			auto params = EventParams::Create();
			params.Insert("sEventName"sv, eventName);
			params.Insert("kArgs"sv, std::move(clonedEventArgs));

			_functionRef.InvokeFunction(std::move(params));
			return RE::BSEventNotifyControl::kContinue;
		}

		auto appliedArgs = std::vector<RE::BSScript::Variable>();
		appliedArgs.reserve(eventArgs.size() + 1);

		appliedArgs.emplace_back(eventName);

		for (const auto& arg : eventArgs) {
			const auto* var = arg.GetVar();
			appliedArgs.push_back(RE::BSScript::UnwrapVariant(var));
		}

		_functionRef.InvokeAppliedFunction(std::move(appliedArgs));
		return RE::BSEventNotifyControl::kContinue;
	}

	void UserEventHandler::RegisterEvents()
	{
		UserEventSource::GetEventSource()->RegisterSink(this);
	}

	void UserEventHandler::UnregisterEvents()
	{
		UserEventSource::GetEventSource()->UnregisterSink(this);
	}
}
