#pragma once

#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRefMap.hpp"

namespace Plugin::Internal::Events
{
	class IEventHandler;

	class EventSystem final
	{
	public:
		using value_type = REX::NotNull<std::shared_ptr<const IEventHandler>>;

		EventSystem() = default;
		~EventSystem() noexcept = default;

		EventSystem(const EventSystem&) = delete;
		EventSystem(EventSystem&&) = delete;

		EventSystem& operator=(const EventSystem&) = delete;
		EventSystem& operator=(EventSystem&&) = delete;

		void OnGameDataReady();

		[[nodiscard]] bool IsPersistent(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRegisteredForAny(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRegistered(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName) const;

		[[nodiscard]] auto GetHandler(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName) const
			-> std::shared_ptr<const IEventHandler>;

		bool Register(const Script::ScriptFunctionRef& a_functionRef, const value_type& a_eventHandler);
		bool Unregister(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName);

		bool UnregisterForAny(const Script::ScriptFunctionRef& a_functionRef);

		bool UnregisterForAllGlobal(const RE::BSFixedString& a_scriptName);
		bool UnregisterForAllLocal(RE::BSScript::ObjectHandle a_objectHandle, const RE::BSFixedString& a_scriptName);
		bool UnregisterForAllLocal(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object);

		void Revert() noexcept;

		bool ForEach(const REX::NotNull<std::function<bool(const value_type&)>>& a_predicate) const;

	private:
		Script::ScriptFunctionRefMap<value_type> _eventHandlers;
	};
}
