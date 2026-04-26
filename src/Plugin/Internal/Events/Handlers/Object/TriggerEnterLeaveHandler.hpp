#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class TriggerEnterLeaveHandler final
		: public EventHandler<
			  TriggerEnterLeaveHandler,
			  TriggerEnterLeaveArgs,
			  TriggerEnterLeaveParams,
			  "OnTriggerEnterLeave">,
		  public RE::BSTEventSink<RE::TESTriggerEnterEvent>,
		  public RE::BSTEventSink<RE::TESTriggerLeaveEvent>
	{
	public:
		TriggerEnterLeaveHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~TriggerEnterLeaveHandler() noexcept override;

		TriggerEnterLeaveHandler(const TriggerEnterLeaveHandler&) = delete;
		TriggerEnterLeaveHandler(TriggerEnterLeaveHandler&&) = delete;

		TriggerEnterLeaveHandler& operator=(const TriggerEnterLeaveHandler&) = delete;
		TriggerEnterLeaveHandler& operator=(TriggerEnterLeaveHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESTriggerEnterEvent& a_event,
			RE::BSTEventSource<RE::TESTriggerEnterEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::TESTriggerLeaveEvent& a_event,
			RE::BSTEventSource<RE::TESTriggerLeaveEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		bool ProcessEventImpl(const RE::NiPointer<RE::TESObjectREFR>& a_sourceRef,
			const RE::NiPointer<RE::TESObjectREFR>& a_targetRef, bool a_isEntered) const;

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetRefFilter;
	};
}
