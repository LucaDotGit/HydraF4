#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class DialogueTargetChangeHandler final
		: public EventHandler<
			  DialogueTargetChangeHandler,
			  DialogueTargetChangeArgs,
			  DialogueTargetChangeParams,
			  "OnDialogueTargetChange">,
		  public RE::BSTEventSink<RE::TESOnPCDialogueTargetEvent>
	{
	public:
		DialogueTargetChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~DialogueTargetChangeHandler() noexcept override;

		DialogueTargetChangeHandler(const DialogueTargetChangeHandler&) = delete;
		DialogueTargetChangeHandler(DialogueTargetChangeHandler&&) = delete;

		DialogueTargetChangeHandler& operator=(const DialogueTargetChangeHandler&) = delete;
		DialogueTargetChangeHandler& operator=(DialogueTargetChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESOnPCDialogueTargetEvent& a_event,
			RE::BSTEventSource<RE::TESOnPCDialogueTargetEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
