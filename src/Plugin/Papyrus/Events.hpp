#pragma once

#include "Plugin/Internal/Events/EventHandler.hpp"
#include "Plugin/Internal/Events/EventSystem.hpp"
#include "Plugin/Internal/EventsManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Structs/FunctionRefs.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::Events
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Events");

	using FunctionRefStruct = ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}

namespace Plugin::Papyrus::Events::Impl
{
	template <::Plugin::Internal::Events::EventHandlerConstraint TEventHandler>
	[[nodiscard]] __forceinline static bool IsRegistered(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		const std::optional<FunctionRefStruct>& a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->IsRegistered(scriptFunctionRef, TEventHandler::EVENT_NAME);
	}

	template <::Plugin::Internal::Events::EventHandlerConstraint TEventHandler>
	[[nodiscard]] __forceinline static bool Register(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		const std::optional<FunctionRefStruct>& a_functionRef,
		const typename TEventHandler::EventArgs& a_args,
		bool a_isPersistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		if constexpr (std::same_as<typename ::Plugin::Internal::Events::event_traits<TEventHandler>::ValidateCallback, std::true_type>) {
			const auto callbackArgs = std::array{ TEventHandler::EventParams::GetTypeInfo() };
			if (!scriptFunctionRef.IsFunctionOfParamTypes(callbackArgs)) [[unlikely]] {
				a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
				return false;
			}
		}

		const auto eventHandler = ::Plugin::Internal::Events::event_functor<TEventHandler>::Create(scriptFunctionRef, a_args.get(), a_isPersistent);
		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->Register(scriptFunctionRef, eventHandler);
	}

	template <::Plugin::Internal::Events::EventHandlerConstraint TEventHandler>
	[[nodiscard]] __forceinline static bool Unregister(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		const std::optional<FunctionRefStruct>& a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->Unregister(scriptFunctionRef, TEventHandler::EVENT_NAME);
	}
}
