#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/AnimationGraphEventSource.hpp"

namespace Plugin::Internal::Events
{
	class AnimationGraphEventHandler final
		: public EventHandler<
			  AnimationGraphEventHandler,
			  AnimationGraphEventArgs,
			  AnimationGraphEventParams,
			  "OnAnimationGraphEvent">,
		  public RE::BSTEventSink<AnimationGraphEvent>
	{
	public:
		AnimationGraphEventHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~AnimationGraphEventHandler() noexcept override;

		AnimationGraphEventHandler(const AnimationGraphEventHandler&) = delete;
		AnimationGraphEventHandler(AnimationGraphEventHandler&&) = delete;

		AnimationGraphEventHandler& operator=(const AnimationGraphEventHandler&) = delete;
		AnimationGraphEventHandler& operator=(AnimationGraphEventHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetEventNameFilter() const noexcept { return _eventNameFilter; }
		[[nodiscard]] const auto& GetPayloadFilter() const noexcept { return _payloadFilter; }

		RE::BSEventNotifyControl ProcessEvent(const AnimationGraphEvent& a_event,
			RE::BSTEventSource<AnimationGraphEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptValueFilter<RE::BSFixedString> _eventNameFilter;
		Script::ScriptValueFilter<RE::BSFixedString> _payloadFilter;
	};
}
