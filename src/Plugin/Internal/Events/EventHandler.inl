#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"

namespace Plugin::Internal::Events
{
	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	EventHandler<EventT, TEventArgs, TEventParams, EventName>::EventHandler(
		const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: IEventHandler(a_functionRef, a_args.get(), a_isPersistent)
	{
	}

	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	auto EventHandler<EventT, TEventArgs, TEventParams, EventName>::Create(
		const Script::ScriptFunctionRef& a_functionRef,
		const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
		bool a_isPersistent)
		-> REX::NotNull<std::shared_ptr<const IEventHandler>>
	{
		return event_functor<EventType>::Create(a_functionRef, a_args, a_isPersistent);
	}

	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	bool EventHandler<EventT, TEventArgs, TEventParams, EventName>::Load(
		const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_outResult)
	{
		return event_functor<EventType>::Load(a_stream, a_outResult);
	}

	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	bool EventHandler<EventT, TEventArgs, TEventParams, EventName>::Save(
		Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler)
	{
		return event_functor<EventType>::Save(a_stream, a_eventHandler);
	}

	template <Impl::EventFunctorConstraint T>
	auto event_functor<T>::Create(
		const Script::ScriptFunctionRef& a_functionRef,
		const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
		bool a_isPersistent)
		-> REX::NotNull<std::shared_ptr<const IEventHandler>>
	{
		const auto eventArgs = typename T::EventArgs(a_args);
		const auto eventHandler = std::make_shared<T>(a_functionRef, eventArgs, a_isPersistent);
		return std::static_pointer_cast<const IEventHandler>(eventHandler);
	}

	template <Impl::EventFunctorConstraint T>
	bool event_functor<T>::Load(
		const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_eventHandler)
	{
		return Impl::Load(a_stream, a_eventHandler, event_functor::Create);
	}

	template <Impl::EventFunctorConstraint T>
	bool event_functor<T>::Save(
		Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler)
	{
		return Impl::Save(a_stream, a_eventHandler);
	}
}
