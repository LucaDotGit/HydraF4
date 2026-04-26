#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class SpellCastHandler final
		: public EventHandler<
			  SpellCastHandler,
			  SpellCastArgs,
			  SpellCastParams,
			  "OnSpellCast">,
		  public RE::BSTEventSink<RE::TESSpellCastEvent>
	{
	public:
		SpellCastHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~SpellCastHandler() noexcept override;

		SpellCastHandler(const SpellCastHandler&) = delete;
		SpellCastHandler(SpellCastHandler&&) = delete;

		SpellCastHandler& operator=(const SpellCastHandler&) = delete;
		SpellCastHandler& operator=(SpellCastHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetSpellFilter() const noexcept { return _spellFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent& a_event,
			RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _spellFilter;
	};
}
