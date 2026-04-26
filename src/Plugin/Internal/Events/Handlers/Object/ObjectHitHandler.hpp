#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	class ObjectHitHandler final
		: public EventHandler<
			  ObjectHitHandler,
			  ObjectHitArgs,
			  ObjectHitParams,
			  "OnObjectHit">,
		  public RE::BSTEventSink<RE::TESHitEvent>
	{
	public:
		using HitData = ObjectHitData;

		ObjectHitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~ObjectHitHandler() noexcept override;

		ObjectHitHandler(const ObjectHitHandler&) = delete;
		ObjectHitHandler(ObjectHitHandler&&) = delete;

		ObjectHitHandler& operator=(const ObjectHitHandler&) = delete;
		ObjectHitHandler& operator=(ObjectHitHandler&&) = delete;

		[[nodiscard]] const auto& GetSourceRefFilter() const noexcept { return _sourceRefFilter; }
		[[nodiscard]] const auto& GetTargetRefFilter() const noexcept { return _targetRefFilter; }
		[[nodiscard]] const auto& GetSourceObjectFilter() const noexcept { return _sourceObjectFilter; }
		[[nodiscard]] const auto& GetSourceProjectileFilter() const noexcept { return _sourceProjectileFilter; }
		[[nodiscard]] const auto& GetMaterialNameFilter() const noexcept { return _materialNameFilter; }
		[[nodiscard]] const auto& GetFlagsFilter() const noexcept { return _flagsFilter; }

		RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent& a_event,
			RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;

	private:
		void RegisterEvents();
		void UnregisterEvents();

		Script::ScriptVariantFilter _sourceRefFilter;
		Script::ScriptVariantFilter _targetRefFilter;
		Script::ScriptVariantFilter _sourceObjectFilter;
		Script::ScriptVariantFilter _sourceProjectileFilter;
		Script::ScriptValueFilter<RE::BSFixedString> _materialNameFilter;
		Script::ScriptFlagsFilter<RE::HitData::Flags> _flagsFilter;
	};
}
