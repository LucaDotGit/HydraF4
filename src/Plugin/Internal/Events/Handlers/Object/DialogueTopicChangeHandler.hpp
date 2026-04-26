#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class DialogueTopicChangeHandler final
		: public EventHandler<
			  DialogueTopicChangeHandler,
			  DialogueTopicChangeArgs,
			  DialogueTopicChangeParams,
			  "OnDialogueTopicChange">,
		  public RE::BSTEventSink<RE::TESTopicInfoEvent>
	{
	public:
		DialogueTopicChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~DialogueTopicChangeHandler() noexcept override;

		DialogueTopicChangeHandler(const DialogueTopicChangeHandler&) = delete;
		DialogueTopicChangeHandler(DialogueTopicChangeHandler&&) = delete;

		DialogueTopicChangeHandler& operator=(const DialogueTopicChangeHandler&) = delete;
		DialogueTopicChangeHandler& operator=(DialogueTopicChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetDialogueTopicFilter() const noexcept { return _dialogTopicFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESTopicInfoEvent& a_event,
			RE::BSTEventSource<RE::TESTopicInfoEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _dialogTopicFilter;
	};
}
