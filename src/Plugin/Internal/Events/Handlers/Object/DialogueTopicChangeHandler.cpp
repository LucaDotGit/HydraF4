#include "Plugin/Internal/Events/Handlers/Object/DialogueTopicChangeHandler.hpp"

namespace Plugin::Internal::Events
{
	DialogueTopicChangeHandler::DialogueTopicChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _dialogTopicFilter(a_args, "kIncludedDialogueTopics"sv, "kExcludedDialogueTopics"sv)
	{
		RegisterEvents();
	}

	DialogueTopicChangeHandler::~DialogueTopicChangeHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl DialogueTopicChangeHandler::ProcessEvent(const RE::TESTopicInfoEvent& a_event,
		RE::BSTEventSource<RE::TESTopicInfoEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.speakerRef.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* dialogTopic = RE::TESForm::FindFormByID<RE::TESTopicInfo>(a_event.topicInfoFormID);
		if (!dialogTopic) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_dialogTopicFilter.IsMatch(dialogTopic)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kDialogueTopic"sv, dialogTopic);
		params.Insert("bStarted"sv, a_event.eventType == RE::TESTopicInfoEvent::EventType::kBegin);

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void DialogueTopicChangeHandler::RegisterEvents()
	{
		RE::TESTopicInfoEvent::GetEventSource()->RegisterSink(this);
	}

	void DialogueTopicChangeHandler::UnregisterEvents()
	{
		RE::TESTopicInfoEvent::GetEventSource()->UnregisterSink(this);
	}
}
