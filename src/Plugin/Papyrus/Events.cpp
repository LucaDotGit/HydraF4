#include "Plugin/Papyrus/Events.hpp"

// NOLINTBEGIN(bugprone-suspicious-include)
#include "Plugin/Papyrus/Events_Actor.cpp"
#include "Plugin/Papyrus/Events_Game.cpp"
#include "Plugin/Papyrus/Events_Object.cpp"
#include "Plugin/Papyrus/Events_Save.cpp"
// NOLINTEND(bugprone-suspicious-include)

#include "Plugin/Internal/Events/EventStructs.hpp"
#include "Plugin/Internal/Events/Handlers/UserEventHandler.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Events::Impl
{
	[[nodiscard]] __forceinline static bool SendUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::BSFixedString a_eventName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_isApplied)
	{
		if (a_eventName.empty()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EVENT_NAME_EMPTY, a_stackId);
			return false;
		}

		const auto& userEventSource = ::Plugin::Internal::Events::UserEventSource::GetEventSource();
		if (!userEventSource->HasSinks()) {
			return false;
		}

		auto userEvent = ::Plugin::Internal::Events::UserEvent{
			.eventName = std::move(a_eventName),
			.args = std::move(a_args),
			.isApplied = a_isApplied
		};

		F4SE::GetTaskInterface()->AddTask([&userEventSource = userEventSource, userEvent = std::move(userEvent)]() {
			userEventSource->Notify(userEvent);
		});

		return true;
	}
}

namespace Plugin::Papyrus::Events
{
	static bool IsPersistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
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

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->IsPersistent(scriptFunctionRef);
	}

	static bool IsRegisteredForAny(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
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

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->IsRegisteredForAny(scriptFunctionRef);
	}

	static bool UnregisterForAny(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
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

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->UnregisterForAny(scriptFunctionRef);
	}

	static bool UnregisterForAllLocal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->UnregisterForAllLocal(a_object);
	}

	static bool UnregisterForAllGlobal(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName)
	{
		return ::Plugin::GetInternalManager()->GetEventsManager()->GetEventSystem()->UnregisterForAllGlobal(a_scriptName);
	}

	static bool IsRegisteredForUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::UserEventHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::UserEventArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::UserEventHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::UserEventHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool SendUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_eventName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		return Impl::SendUserEvent(a_vm, a_stackId, std::move(a_eventName), std::move(a_args), false);
	}

	static bool SendAppliedUserEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_eventName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		return Impl::SendUserEvent(a_vm, a_stackId, std::move(a_eventName), std::move(a_args), true);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPersistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForAny);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForAny);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForAllLocal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForAllGlobal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForUserEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForUserEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForUserEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SendUserEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SendAppliedUserEvent);

		Events::Save::RegisterFunctions(a_vm);
		Events::Actor::RegisterFunctions(a_vm);
		Events::Object::RegisterFunctions(a_vm);
		Events::Game::RegisterFunctions(a_vm);
	}
}
