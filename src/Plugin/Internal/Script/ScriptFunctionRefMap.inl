#pragma once

#include "Plugin/Internal/Script/ScriptFunctionRefMap.hpp"

namespace Plugin::Internal::Script::Impl
{
	[[nodiscard]] bool IsInScript(
		const RE::BSFixedString& a_scriptName,
		const ScriptFunctionRef& a_functionRef) noexcept;
}

namespace Plugin::Internal::Script
{
	template <class T>
	ScriptFunctionRefMap<T>::ScriptFunctionRefMap() = default;

	template <class T>
	ScriptFunctionRefMap<T>::~ScriptFunctionRefMap() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::size() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _functionRefMap.size();
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::capacity() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _functionRefMap.capacity();
	}

	template <class T>
	constexpr ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::max_size() const noexcept
	{
		return _functionRefMap.max_size();
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::empty() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _functionRefMap.empty();
	}

	template <class T>
	void ScriptFunctionRefMap<T>::RegisterEvents()
	{
		auto* activeEffectApplyRemoveEventSource = RE::TESActiveEffectApplyRemoveEvent::GetEventSource();
		if (activeEffectApplyRemoveEventSource) {
			activeEffectApplyRemoveEventSource->RegisterSink(this);
		}

		auto* questStartStopEventSource = RE::TESQuestStartStopEvent::GetEventSource();
		if (questStartStopEventSource) {
			questStartStopEventSource->RegisterSink(this);
		}

		auto* formDeleteEventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (formDeleteEventSource) {
			formDeleteEventSource->RegisterSink(this);
		}

		auto* formIdRemapEventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (formIdRemapEventSource) {
			formIdRemapEventSource->RegisterSink(this);
		}

		auto* inputLayerManager = RE::BSInputEnableManager::GetSingleton();
		if (inputLayerManager) {
			auto* eventSource = static_cast<RE::BSTEventSource<RE::InputEnableLayerDestroyedEvent>*>(inputLayerManager);
			if (eventSource) {
				eventSource->RegisterSink(this);
			}
		}
	}

	template <class T>
	void ScriptFunctionRefMap<T>::UnregisterEvents()
	{
		auto* activeEffectApplyRemoveEventSource = RE::TESActiveEffectApplyRemoveEvent::GetEventSource();
		if (activeEffectApplyRemoveEventSource) {
			activeEffectApplyRemoveEventSource->UnregisterSink(this);
		}

		auto* questStartStopEventSource = RE::TESQuestStartStopEvent::GetEventSource();
		if (questStartStopEventSource) {
			questStartStopEventSource->UnregisterSink(this);
		}

		auto* formDeleteEventSource = RE::TESFormDeleteEvent::GetEventSource();
		if (formDeleteEventSource) {
			formDeleteEventSource->UnregisterSink(this);
		}

		auto* formIdRemapEventSource = RE::TESFormIDRemapEvent::GetEventSource();
		if (formIdRemapEventSource) {
			formIdRemapEventSource->UnregisterSink(this);
		}

		auto* inputLayerManager = RE::BSInputEnableManager::GetSingleton();
		if (inputLayerManager) {
			auto* eventSource = static_cast<RE::BSTEventSource<RE::InputEnableLayerDestroyedEvent>*>(inputLayerManager);
			if (eventSource) {
				eventSource->UnregisterSink(this);
			}
		}
	}

	template <class T>
	RE::BSEventNotifyControl ScriptFunctionRefMap<T>::ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>* a_eventSource)
	{
		if (a_event.applied) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.targetRef.get();
		if (!targetRef) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForActiveEffect(
			a_event.activeEffectUniqueID, targetRef->GetFormID());
		RemoveAll(objectHandle);

		return RE::BSEventNotifyControl::kContinue;
	}

	template <class T>
	RE::BSEventNotifyControl ScriptFunctionRefMap<T>::ProcessEvent(const RE::TESQuestStartStopEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_eventSource)
	{
		if (a_event.started) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto questFormId = a_event.questFormID;
		if (questFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto handleLock = std::scoped_lock(_mapMutex);

		const auto questHandle = RE::GameScript::HandlePolicy::GetHandleForForm(questFormId);
		RemoveAllImpl(questHandle);

		const auto* quest = RE::TESForm::FindFormByNumericID<RE::TESQuest>(questFormId);
		if (!quest) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto aliasLock = RE::BSAutoReadLock(quest->aliasAccessLock);

		for (const auto* alias : quest->aliases) {
			if (!alias) {
				continue;
			}

			const auto aliasHandle = RE::GameScript::HandlePolicy::GetHandleForAlias(
				static_cast<std::uint16_t>(alias->aliasID), questFormId);
			RemoveAllImpl(aliasHandle);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	template <class T>
	RE::BSEventNotifyControl ScriptFunctionRefMap<T>::ProcessEvent(const RE::TESFormDeleteEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource)
	{
		const auto formId = a_event.formID;
		if (formId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto objectHandle = RE::GameScript::HandlePolicy::GetHandleForForm(formId);
		RemoveAll(objectHandle);

		return RE::BSEventNotifyControl::kContinue;
	}

	template <class T>
	RE::BSEventNotifyControl ScriptFunctionRefMap<T>::ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource)
	{
		const auto oldFormId = a_event.oldFormID;
		if (oldFormId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto oldObjectHandle = RE::GameScript::HandlePolicy::GetHandleForForm(oldFormId);
		RemoveAll(oldObjectHandle);

		return RE::BSEventNotifyControl::kContinue;
	}

	template <class T>
	RE::BSEventNotifyControl ScriptFunctionRefMap<T>::ProcessEvent(const RE::InputEnableLayerDestroyedEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::InputEnableLayerDestroyedEvent>* a_eventSource)
	{
		const auto layerId = a_event.layerID;
		if (layerId == std::numeric_limits<std::uint32_t>::max()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto layerHandle = RE::GameScript::HandlePolicy::GetHandleForInputEnableLayer(layerId);
		RemoveAll(layerHandle);

		return RE::BSEventNotifyControl::kContinue;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Contains(
		const ScriptFunctionRef& a_functionRef) const
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto& functionRefMap = objectHandleIt->second;
		return functionRefMap.contains(a_functionRef);
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Contains(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return false;
		}

		return std::invoke(*a_valuePredicate, a_functionRef, functionRefIt->second);
	}

	template <class T>
	auto ScriptFunctionRefMap<T>::GetValue(
		const ScriptFunctionRef& a_functionRef) const -> std::optional<value_type>
		requires(std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return std::nullopt;
		}

		const auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return std::nullopt;
		}

		return functionRefIt->second;
	}

	template <class T>
	auto ScriptFunctionRefMap<T>::GetValue(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const -> std::optional<value_type>
		requires(std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return std::nullopt;
		}

		const auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return std::nullopt;
		}

		if (!std::invoke(*a_valuePredicate, a_functionRef, functionRefIt->second)) {
			return std::nullopt;
		}

		return functionRefIt->second;
	}

	template <class T>
	ScriptFunctionRefMap<T>::value_type ScriptFunctionRefMap<T>::GetValueOrDefault(
		const ScriptFunctionRef& a_functionRef) const
		requires(std::is_default_constructible_v<value_type> &&
				 std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return {};
		}

		const auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return {};
		}

		return functionRefIt->second;
	}

	template <class T>
	ScriptFunctionRefMap<T>::value_type ScriptFunctionRefMap<T>::GetValueOrDefault(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const
		requires(std::is_default_constructible_v<value_type> &&
				 std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return {};
		}

		const auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return {};
		}

		if (!std::invoke(*a_valuePredicate, a_functionRef, functionRefIt->second)) {
			return {};
		}

		return functionRefIt->second;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Add(
		const ScriptFunctionRef& a_functionRef,
		const value_type& a_value)
		requires(std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto&& [objectHandleIt, _] = _functionRefMap.try_emplace(a_functionRef.GetObjectHandle());
		auto&& [functionRefIt, hasInserted] = objectHandleIt->second.try_emplace(a_functionRef, a_value);
		return hasInserted;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Add(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<value_type(const ScriptFunctionRef&)>>& a_valueFactory)
		requires(std::is_default_constructible_v<value_type> &&
				 (std::is_copy_constructible_v<value_type> ||
					 std::is_move_constructible_v<value_type>))
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto&& [objectHandleIt, _] = _functionRefMap.try_emplace(a_functionRef.GetObjectHandle());
		auto&& [functionRefIt, hasInserted] = objectHandleIt->second.try_emplace(a_functionRef);

		if (!hasInserted) {
			return false;
		}

		functionRefIt->second = std::invoke(*a_valueFactory, a_functionRef);
		return true;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Update(
		const ScriptFunctionRef& a_functionRef,
		const value_type& a_value)
		requires(std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto functionRefIt = objectHandleIt->second.find(a_functionRef);
		if (functionRefIt == objectHandleIt->second.end()) {
			return false;
		}

		functionRefIt->second = a_value;
		return true;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Update(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<value_type(const ScriptFunctionRef&, value_type*)>>& a_valueFactory)
		requires(std::is_copy_constructible_v<value_type> ||
				 std::is_move_constructible_v<value_type>)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto functionRefIt = objectHandleIt->second.find(a_functionRef);
		if (functionRefIt == objectHandleIt->second.end()) {
			return false;
		}

		functionRefIt->second = std::invoke(*a_valueFactory, a_functionRef, std::addressof(functionRefIt->second));
		return true;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::AddOrUpdate(
		const ScriptFunctionRef& a_functionRef,
		const value_type& a_value)
		requires(std::is_copy_constructible_v<value_type>)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto&& [objectHandleIt, _] = _functionRefMap.try_emplace(a_functionRef.GetObjectHandle());
		auto&& [functionRefIt, hasInserted] = objectHandleIt->second.insert_or_assign(a_functionRef, a_value);
		return hasInserted;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::AddOrUpdate(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<value_type(const ScriptFunctionRef&, value_type*)>>& a_valueFactory)
		requires(std::is_default_constructible_v<value_type> &&
				 (std::is_copy_constructible_v<value_type> ||
					 std::is_move_constructible_v<value_type>))
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto&& [objectHandleIt, _] = _functionRefMap.try_emplace(a_functionRef.GetObjectHandle());
		auto&& [functionRefIt, hasInserted] = objectHandleIt->second.try_emplace(a_functionRef);

		functionRefIt->second = std::invoke(*a_valueFactory, a_functionRef, hasInserted ? nullptr : std::addressof(functionRefIt->second));
		return hasInserted;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Remove(
		const ScriptFunctionRef& a_functionRef)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto functionRefIt = objectHandleIt->second.find(a_functionRef);
		if (functionRefIt == objectHandleIt->second.end()) {
			return false;
		}

		objectHandleIt->second.erase(functionRefIt);
		return true;
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::Remove(
		const ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_functionRef.GetObjectHandle());
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		auto& functionRefMap = objectHandleIt->second;

		const auto functionRefIt = functionRefMap.find(a_functionRef);
		if (functionRefIt == functionRefMap.end()) {
			return false;
		}

		if (!std::invoke(*a_valuePredicate, functionRefIt->first, functionRefIt->second)) {
			return false;
		}

		functionRefMap.erase(functionRefIt);
		return true;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAll(
		RE::BSScript::ObjectHandle a_objectHandle)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		return RemoveAllImpl(a_objectHandle);
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAll(
		RE::BSScript::ObjectHandle a_objectHandle,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		return RemoveAllImpl(a_objectHandle, a_valuePredicate);
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAll(
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto removedTotalCount = static_cast<size_type>(0);
		std::erase_if(_functionRefMap, [&a_valuePredicate, &removedTotalCount](auto& a_objectHandleMapPair) {
			auto& functionRefMap = a_objectHandleMapPair.second;

			const auto removedCount = std::erase_if(functionRefMap, [&a_valuePredicate](auto& a_functionRefPair) {
				return std::invoke(*a_valuePredicate, a_functionRefPair.first, a_functionRefPair.second);
			});

			removedTotalCount += removedCount;
			return functionRefMap.empty();
		});

		return removedTotalCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		const RE::BSFixedString& a_scriptName)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto removedTotalCount = static_cast<size_type>(0);
		std::erase_if(_functionRefMap, [&a_scriptName, &removedTotalCount](auto& a_objectHandleMapPair) {
			auto& functionRefMap = a_objectHandleMapPair.second;

			const auto removedCount = std::erase_if(functionRefMap, [&a_scriptName](auto& a_functionRefPair) {
				return Impl::IsInScript(a_scriptName, a_functionRefPair.first);
			});

			removedTotalCount += removedCount;
			return functionRefMap.empty();
		});

		return removedTotalCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		const RE::BSFixedString& a_scriptName,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		auto removedTotalCount = static_cast<size_type>(0);
		std::erase_if(_functionRefMap, [&a_scriptName, &a_valuePredicate, &removedTotalCount](auto& a_objectHandleMapPair) {
			auto& functionRefMap = a_objectHandleMapPair.second;

			const auto removedCount = std::erase_if(functionRefMap, [&a_scriptName, &a_valuePredicate](auto& a_functionRefPair) {
				auto& functionRef = a_functionRefPair.first;
				if (!Impl::IsInScript(a_scriptName, functionRef)) {
					return false;
				}

				return std::invoke(*a_valuePredicate, functionRef, a_functionRefPair.second);
			});

			removedTotalCount += removedCount;
			return functionRefMap.empty();
		});

		return removedTotalCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		RE::BSScript::ObjectHandle a_objectHandle,
		const RE::BSFixedString& a_scriptName)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_objectHandle);
		if (objectHandleIt == _functionRefMap.end()) {
			return 0;
		}

		auto& functionRefMap = objectHandleIt->second;

		const auto removedCount = std::erase_if(functionRefMap, [&a_scriptName](auto& a_functionRefPair) {
			return Impl::IsInScript(a_scriptName, a_functionRefPair.first);
		});

		if (functionRefMap.empty()) {
			_functionRefMap.erase(objectHandleIt);
		}

		return removedCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		RE::BSScript::ObjectHandle a_objectHandle,
		const RE::BSFixedString& a_scriptName,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_objectHandle);
		if (objectHandleIt == _functionRefMap.end()) {
			return 0;
		}

		auto& functionRefMap = objectHandleIt->second;

		const auto removedCount = std::erase_if(functionRefMap, [&a_scriptName, &a_valuePredicate](auto& a_functionRefPair) {
			auto& functionRef = a_functionRefPair.first;
			if (!Impl::IsInScript(a_scriptName, functionRef)) {
				return false;
			}

			return std::invoke(*a_valuePredicate, functionRef, a_functionRefPair.second);
		});

		if (functionRefMap.empty()) {
			_functionRefMap.erase(objectHandleIt);
		}

		return removedCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		const RE::BSTSmartPointer<RE::BSScript::Object>& a_object)
	{
		if (!a_object) {
			return 0;
		}

		const auto objectHandle = a_object->GetHandle();
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return 0;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) {
			return 0;
		}

		return RemoveAllInScript(objectHandle, scriptName.value());
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllInScript(
		const RE::BSTSmartPointer<RE::BSScript::Object>& a_object,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate)
	{
		if (!a_object) {
			return 0;
		}

		const auto objectHandle = a_object->GetHandle();
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return 0;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) {
			return 0;
		}

		return RemoveAllInScript(objectHandle, scriptName.value(), a_valuePredicate);
	}

	template <class T>
	void ScriptFunctionRefMap<T>::Reserve(
		ScriptFunctionRefMap<T>::size_type a_size)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		_functionRefMap.reserve(a_size);
	}

	template <class T>
	void ScriptFunctionRefMap<T>::Clear()
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		_functionRefMap.clear();
		_functionRefMap.rehash(0);
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::ForEach(
		const REX::NotNull<std::function<bool(const value_type&)>>& a_valueCallback) const
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		return std::ranges::all_of(_functionRefMap, [&a_valueCallback](const auto& a_elementPair) {
			return std::ranges::all_of(a_elementPair.second, [&a_valueCallback](const auto& a_functionRefPair) {
				return std::invoke(*a_valueCallback, a_functionRefPair.second);
			});
		});
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::ForEachInScript(
		const RE::BSFixedString& a_scriptName,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		return std::ranges::all_of(_functionRefMap, [&a_scriptName, &a_valueCallback](const auto& a_elementPair) {
			return std::ranges::all_of(a_elementPair.second, [&a_scriptName, &a_valueCallback](const auto& a_functionRefPair) {
				const auto& functionRef = a_functionRefPair.first;
				if (!Impl::IsInScript(a_scriptName, functionRef)) {
					return true;
				}

				return std::invoke(*a_valueCallback, functionRef, a_functionRefPair.second);
			});
		});
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::ForEachInScript(
		RE::BSScript::ObjectHandle a_objectHandle,
		const RE::BSFixedString& a_scriptName,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const
	{
		const auto mapLock = std::shared_lock(_mapMutex);

		const auto objectHandleIt = _functionRefMap.find(a_objectHandle);
		if (objectHandleIt == _functionRefMap.end()) {
			return false;
		}

		const auto& functionRefMap = objectHandleIt->second;

		return std::ranges::all_of(functionRefMap, [&a_scriptName, &a_valueCallback](const auto& a_functionRefPair) {
			const auto& functionRef = a_functionRefPair.first;
			if (!Impl::IsInScript(a_scriptName, functionRef)) {
				return true;
			}

			return std::invoke(*a_valueCallback, functionRef, a_functionRefPair.second);
		});
	}

	template <class T>
	bool ScriptFunctionRefMap<T>::ForEachInScript(
		const RE::BSTSmartPointer<RE::BSScript::Object>& a_object,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const
	{
		if (!a_object) {
			return false;
		}

		const auto objectHandle = a_object->GetHandle();
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return false;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) {
			return false;
		}

		return ForEachInScript(objectHandle, scriptName.value(), a_valueCallback);
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllImpl(
		RE::BSScript::ObjectHandle a_objectHandle)
	{
		const auto objectHandleIt = _functionRefMap.find(a_objectHandle);
		if (objectHandleIt == _functionRefMap.end()) {
			return 0;
		}

		const auto removedCount = objectHandleIt->second.size();
		_functionRefMap.erase(objectHandleIt);
		return removedCount;
	}

	template <class T>
	ScriptFunctionRefMap<T>::size_type ScriptFunctionRefMap<T>::RemoveAllImpl(
		RE::BSScript::ObjectHandle a_objectHandle,
		const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate)
	{
		const auto objectHandleIt = _functionRefMap.find(a_objectHandle);
		if (objectHandleIt == _functionRefMap.end()) {
			return 0;
		}

		auto& functionRefMap = objectHandleIt->second;

		const auto removedCount = std::erase_if(functionRefMap, [&a_valuePredicate](const auto& a_functionRefPair) {
			return std::invoke(*a_valuePredicate, a_functionRefPair.first, a_functionRefPair.second);
		});

		if (functionRefMap.empty()) {
			_functionRefMap.erase(objectHandleIt);
		}

		return removedCount;
	}
}
