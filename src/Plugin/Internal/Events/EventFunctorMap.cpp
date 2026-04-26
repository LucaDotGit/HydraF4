#include "Plugin/Internal/Events/EventFunctorMap.hpp"

namespace Plugin::Internal::Events
{
	EventFunctorMap::CallbackData::CallbackData(
		REX::NotNull<std::function<GetEventArgsNameType>> a_getEventArgsNameCallback,
		REX::NotNull<std::function<GetEventParamsNameType>> a_getEventParamsNameCallback,
		REX::NotNull<std::function<CreateFunctorType>> a_createCallback,
		REX::NotNull<std::function<LoadFunctorType>> a_loadCallback,
		REX::NotNull<std::function<SaveFunctorType>> a_saveCallback)
		: _getEventArgsNameCallback(std::move(a_getEventArgsNameCallback)),
		  _getEventParamsNameCallback(std::move(a_getEventParamsNameCallback)),
		  _createCallback(std::move(a_createCallback)),
		  _loadCallback(std::move(a_loadCallback)),
		  _saveCallback(std::move(a_saveCallback))
	{
	}

	bool EventFunctorMap::Contains(std::string_view a_eventName) const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _callbackDataMap.contains(a_eventName);
	}

	auto EventFunctorMap::GetValue(std::string_view a_eventName) const -> std::shared_ptr<CallbackData>
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto callbackDataIt = _callbackDataMap.find(a_eventName);
		if (callbackDataIt == _callbackDataMap.end()) {
			return nullptr;
		}

		return callbackDataIt->second.get();
	}

	void EventFunctorMap::SetValue(std::string_view a_eventName, const REX::NotNull<std::shared_ptr<CallbackData>>& a_callbacks)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		_callbackDataMap.insert_or_assign(a_eventName, a_callbacks);
	}

	bool EventFunctorMap::Add(std::string_view a_eventName, const REX::NotNull<std::shared_ptr<CallbackData>>& a_callbacks)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		const auto&& [_, hasInserted] = _callbackDataMap.emplace(a_eventName, a_callbacks);
		return hasInserted;
	}

	bool EventFunctorMap::Remove(std::string_view a_eventName) noexcept
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto callbackDataIt = _callbackDataMap.find(a_eventName);
		if (callbackDataIt == _callbackDataMap.end()) {
			return false;
		}

		_callbackDataMap.erase(callbackDataIt);
		return true;
	}

	void EventFunctorMap::Clear() noexcept
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		_callbackDataMap.clear();
		_callbackDataMap.rehash(0);
	}

	auto GetEventFunctorMap() -> const REX::NotNull<std::unique_ptr<EventFunctorMap>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<EventFunctorMap>());
		return INSTANCE;
	}
}
