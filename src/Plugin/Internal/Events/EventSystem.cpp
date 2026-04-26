#include "Plugin/Internal/Events/EventSystem.hpp"

#include "Plugin/Internal/Events/EventFunctorMap.hpp"
#include "Plugin/Internal/Events/IEventHandler.hpp"

namespace Plugin::Internal::Events
{
	void EventSystem::OnGameDataReady()
	{
		_eventHandlers.RegisterEvents();
	}

	bool EventSystem::IsPersistent(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto eventHandler = _eventHandlers.GetValue(a_functionRef);
		if (!eventHandler.has_value()) {
			return false;
		}

		return (*eventHandler)->GetIsPersistent();
	}

	bool EventSystem::IsRegisteredForAny(const Script::ScriptFunctionRef& a_functionRef) const
	{
		return _eventHandlers.Contains(a_functionRef);
	}

	bool EventSystem::IsRegistered(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName) const
	{
		const auto eventHandler = _eventHandlers.GetValue(a_functionRef);
		if (!eventHandler.has_value()) {
			return false;
		}

		return (*eventHandler)->GetName() == a_eventName;
	}

	std::shared_ptr<const IEventHandler> EventSystem::GetHandler(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName) const
	{
		auto eventHandler = _eventHandlers.GetValue(a_functionRef);
		if (!eventHandler.has_value()) {
			return nullptr;
		}

		if ((*eventHandler)->GetName() != a_eventName) {
			return nullptr;
		}

		return std::move(eventHandler->get());
	}

	bool EventSystem::Register(const Script::ScriptFunctionRef& a_functionRef, const value_type& a_eventHandler)
	{
		return _eventHandlers.Add(a_functionRef, a_eventHandler);
	}

	bool EventSystem::Unregister(const Script::ScriptFunctionRef& a_functionRef, std::string_view a_eventName)
	{
		return _eventHandlers.Remove(a_functionRef,
			[a_eventName]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, const value_type& a_eventHandler) {
				return a_eventHandler->GetName() == a_eventName;
			});
	}

	bool EventSystem::UnregisterForAny(const Script::ScriptFunctionRef& a_functionRef)
	{
		return _eventHandlers.Remove(a_functionRef);
	}

	bool EventSystem::UnregisterForAllGlobal(const RE::BSFixedString& a_scriptName)
	{
		return _eventHandlers.RemoveAllInScript(a_scriptName) != 0;
	}

	bool EventSystem::UnregisterForAllLocal(RE::BSScript::ObjectHandle a_objectHandle, const RE::BSFixedString& a_scriptName)
	{
		return _eventHandlers.RemoveAllInScript(a_objectHandle, a_scriptName) != 0;
	}

	bool EventSystem::UnregisterForAllLocal(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object)
	{
		return _eventHandlers.RemoveAllInScript(a_object) != 0;
	}

	void EventSystem::Revert() noexcept
	{
		_eventHandlers.Clear();
	}

	bool EventSystem::ForEach(const REX::NotNull<std::function<bool(const value_type&)>>& a_predicate) const
	{
		return _eventHandlers.ForEach(a_predicate);
	}
}
