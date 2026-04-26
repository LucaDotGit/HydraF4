#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class DestructionStageChangeHandler final
		: public EventHandler<
			  DestructionStageChangeHandler,
			  DestructionStageChangeArgs,
			  DestructionStageChangeParams,
			  "OnDestructionStageChange">,
		  public RE::BSTEventSink<RE::TESDestructionStageChangedEvent>
	{
	public:
		DestructionStageChangeHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~DestructionStageChangeHandler() noexcept override;

		DestructionStageChangeHandler(const DestructionStageChangeHandler&) = delete;
		DestructionStageChangeHandler(DestructionStageChangeHandler&&) = delete;

		DestructionStageChangeHandler& operator=(const DestructionStageChangeHandler&) = delete;
		DestructionStageChangeHandler& operator=(DestructionStageChangeHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetOldStageFilter() const noexcept { return _oldStageFilter; }
		[[nodiscard]] const auto& GetNewStageFilter() const noexcept { return _newStageFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESDestructionStageChangedEvent& a_event,
			RE::BSTEventSource<RE::TESDestructionStageChangedEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptValueFilter<std::uint32_t> _oldStageFilter;
		Script::ScriptValueFilter<std::uint32_t> _newStageFilter;
	};
}
