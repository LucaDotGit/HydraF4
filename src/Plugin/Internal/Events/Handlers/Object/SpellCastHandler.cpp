#include "Plugin/Internal/Events/Handlers/Object/SpellCastHandler.hpp"

namespace Plugin::Internal::Events
{
	SpellCastHandler::SpellCastHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _spellFilter(a_args, "kIncludedSpells"sv, "kExcludedSpells"sv)
	{
		RegisterEvents();
	}

	SpellCastHandler::~SpellCastHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl SpellCastHandler::ProcessEvent(const RE::TESSpellCastEvent& a_event,
		RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* sourceRef = a_event.casterRef.get();
		if (!sourceRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* spell = RE::TESForm::FindFormByNumericID<RE::SpellItem>(a_event.spellFormID);
		if (!spell) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_spellFilter.IsMatch(spell)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kSpell"sv, spell);

		if (!_functionRef.InvokeFunction(std::move(params))) [[unlikely]] {
			a_eventSource->UnregisterSink(this);
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	void SpellCastHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESSpellCastEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void SpellCastHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESSpellCastEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
