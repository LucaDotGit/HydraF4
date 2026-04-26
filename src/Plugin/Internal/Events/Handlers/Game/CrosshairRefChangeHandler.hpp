#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/Sources/CrosshairRefChangeSource.hpp"

namespace Plugin::Internal::Events
{
	class CrosshairRefChangeHandler final
		: public EventHandler<
			  CrosshairRefChangeHandler,
			  CrosshairRefChangeArgs,
			  CrosshairRefChangeParams,
			  "OnCrosshairRefChange">,
		  public RE::BSTEventSink<CrosshairRefChangeEvent>
	{
	public:
		CrosshairRefChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~CrosshairRefChangeHandler() noexcept override;

		CrosshairRefChangeHandler(const CrosshairRefChangeHandler&) = delete;
		CrosshairRefChangeHandler(CrosshairRefChangeHandler&&) = delete;

		CrosshairRefChangeHandler& operator=(const CrosshairRefChangeHandler&) = delete;
		CrosshairRefChangeHandler& operator=(CrosshairRefChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }

		RE::BSEventNotifyControl ProcessEvent(const CrosshairRefChangeEvent& a_event,
			RE::BSTEventSource<CrosshairRefChangeEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _targetRefFilter;
	};
}
