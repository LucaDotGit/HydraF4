#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class PerkEntryRunHandler final
		: public EventHandler<
			  PerkEntryRunHandler,
			  PerkEntryRunArgs,
			  PerkEntryRunParams,
			  "OnPerkEntryRun">,
		  public RE::BSTEventSink<RE::TESPerkEntryRunEvent>
	{
	public:
		PerkEntryRunHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~PerkEntryRunHandler() noexcept override;

		PerkEntryRunHandler(const PerkEntryRunHandler&) = delete;
		PerkEntryRunHandler(PerkEntryRunHandler&&) = delete;

		PerkEntryRunHandler& operator=(const PerkEntryRunHandler&) = delete;
		PerkEntryRunHandler& operator=(PerkEntryRunHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceActorFilter() const noexcept { return _sourceActorFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }
		[[nodiscard]] const auto& GetPerkFilter() const noexcept { return _perkFilter; }
		[[nodiscard]] const auto& GetEntryIdFilter() const noexcept { return _entryIdFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESPerkEntryRunEvent& a_event,
			RE::BSTEventSource<RE::TESPerkEntryRunEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceActorFilter;
		Script::ScriptVariantFilter _targetRefFilter;
		Script::ScriptVariantFilter _perkFilter;
		Script::ScriptValueFilter<std::uint16_t> _entryIdFilter;
	};
}
