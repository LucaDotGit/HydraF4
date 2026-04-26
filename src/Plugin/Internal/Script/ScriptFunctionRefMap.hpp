#pragma once

#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Script
{
	template <class T>
	class ScriptFunctionRefMap final
		: public RE::BSTEventSink<RE::TESActiveEffectApplyRemoveEvent>,
		  public RE::BSTEventSink<RE::TESQuestStartStopEvent>,
		  public RE::BSTEventSink<RE::TESFormDeleteEvent>,
		  public RE::BSTEventSink<RE::TESFormIDRemapEvent>,
		  public RE::BSTEventSink<RE::InputEnableLayerDestroyedEvent>
	{
	public:
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		ScriptFunctionRefMap();
		~ScriptFunctionRefMap() noexcept override;

		ScriptFunctionRefMap(const ScriptFunctionRefMap&) = delete;
		ScriptFunctionRefMap(ScriptFunctionRefMap&&) = delete;

		ScriptFunctionRefMap& operator=(const ScriptFunctionRefMap&) = delete;
		ScriptFunctionRefMap& operator=(ScriptFunctionRefMap&&) = delete;

		[[nodiscard]] size_type size() const noexcept;
		[[nodiscard]] size_type capacity() const noexcept;
		[[nodiscard]] constexpr size_type max_size() const noexcept;

		[[nodiscard]] bool empty() const noexcept;

		void RegisterEvents();
		void UnregisterEvents();

		RE::BSEventNotifyControl ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent& a_event,
			RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::TESQuestStartStopEvent& a_event,
			RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormDeleteEvent& a_event,
			RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
			RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::InputEnableLayerDestroyedEvent& a_event,
			RE::BSTEventSource<RE::InputEnableLayerDestroyedEvent>* a_eventSource) override;

		[[nodiscard]] bool Contains(
			const ScriptFunctionRef& a_functionRef) const;

		[[nodiscard]] bool Contains(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const;

		[[nodiscard]] auto GetValue(
			const ScriptFunctionRef& a_functionRef) const -> std::optional<value_type>
			requires(std::is_copy_constructible_v<value_type>);

		[[nodiscard]] auto GetValue(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const -> std::optional<value_type>
			requires(std::is_copy_constructible_v<value_type>);

		[[nodiscard]] value_type GetValueOrDefault(
			const ScriptFunctionRef& a_functionRef) const
			requires(std::is_default_constructible_v<value_type> &&
					 std::is_copy_constructible_v<value_type>);

		[[nodiscard]] value_type GetValueOrDefault(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate) const
			requires(std::is_default_constructible_v<value_type> &&
					 std::is_copy_constructible_v<value_type>);

		bool Add(
			const ScriptFunctionRef& a_functionRef,
			const value_type& a_value)
			requires(std::is_copy_constructible_v<value_type>);

		bool Add(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<value_type(const ScriptFunctionRef&)>>& a_valueFactory)
			requires(std::is_default_constructible_v<value_type> &&
					 (std::is_copy_constructible_v<value_type> ||
						 std::is_move_constructible_v<value_type>));

		bool Update(
			const ScriptFunctionRef& a_functionRef,
			const value_type& a_value)
			requires(std::is_copy_constructible_v<value_type>);

		bool Update(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<value_type(const ScriptFunctionRef&, value_type*)>>& a_valueFactory)
			requires(std::is_copy_constructible_v<value_type> ||
					 std::is_move_constructible_v<value_type>);

		bool AddOrUpdate(
			const ScriptFunctionRef& a_functionRef,
			const value_type& a_value)
			requires(std::is_copy_constructible_v<value_type>);

		bool AddOrUpdate(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<value_type(const ScriptFunctionRef&, value_type*)>>& a_valueFactory)
			requires(std::is_default_constructible_v<value_type> &&
					 (std::is_copy_constructible_v<value_type> ||
						 std::is_move_constructible_v<value_type>));

		bool Remove(
			const ScriptFunctionRef& a_functionRef);

		bool Remove(
			const ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		size_type RemoveAll(
			RE::BSScript::ObjectHandle a_objectHandle);

		size_type RemoveAll(
			RE::BSScript::ObjectHandle a_objectHandle,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		size_type RemoveAll(
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		size_type RemoveAllInScript(
			const RE::BSFixedString& a_scriptName);

		size_type RemoveAllInScript(
			const RE::BSFixedString& a_scriptName,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		size_type RemoveAllInScript(
			RE::BSScript::ObjectHandle a_objectHandle,
			const RE::BSFixedString& a_scriptName);

		size_type RemoveAllInScript(
			RE::BSScript::ObjectHandle a_objectHandle,
			const RE::BSFixedString& a_scriptName,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		size_type RemoveAllInScript(
			const RE::BSTSmartPointer<RE::BSScript::Object>& a_object);

		size_type RemoveAllInScript(
			const RE::BSTSmartPointer<RE::BSScript::Object>& a_object,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, value_type&)>>& a_valuePredicate);

		void Reserve(size_type a_size);

		void Clear();

		bool ForEach(
			const REX::NotNull<std::function<bool(const value_type&)>>& a_valueCallback) const;

		bool ForEachInScript(
			const RE::BSFixedString& a_scriptName,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const;

		bool ForEachInScript(
			RE::BSScript::ObjectHandle a_objectHandle,
			const RE::BSFixedString& a_scriptName,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const;

		bool ForEachInScript(
			const RE::BSTSmartPointer<RE::BSScript::Object>& a_object,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valueCallback) const;

	private:
		size_type RemoveAllImpl(
			RE::BSScript::ObjectHandle a_objectHandle);

		size_type RemoveAllImpl(
			RE::BSScript::ObjectHandle a_objectHandle,
			const REX::NotNull<std::function<bool(const ScriptFunctionRef&, const value_type&)>>& a_valuePredicate);

		mutable std::shared_mutex _mapMutex;
		std::unordered_map<RE::BSScript::ObjectHandle, std::unordered_map<ScriptFunctionRef, value_type>> _functionRefMap;
	};
}

#include "Plugin/Internal/Script/ScriptFunctionRefMap.inl"
