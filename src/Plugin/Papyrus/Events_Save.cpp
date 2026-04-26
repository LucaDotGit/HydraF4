#include "Plugin/Papyrus/Events.hpp"

#include "Plugin/Internal/Events/EventStructs.hpp"

#include "Plugin/Internal/Events/Handlers/Save/DeleteGameHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Save/NewGameHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Save/PostLoadGameHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Save/PostSaveGameHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Save/StartGameHandler.hpp"

namespace Plugin::Papyrus::Events::Save
{
	static bool IsRegisteredForDeleteGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::DeleteGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForDeleteGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::DeleteGameArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::DeleteGameHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForDeleteGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::DeleteGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForNewGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::NewGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForNewGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::NewGameArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::NewGameHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForNewGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::NewGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPostLoadGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PostLoadGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPostLoadGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PostLoadGameArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PostLoadGameHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPostLoadGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PostLoadGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPostSaveGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PostSaveGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPostSaveGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PostSaveGameArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PostSaveGameHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPostSaveGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PostSaveGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForStartGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::StartGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForStartGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::StartGameArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::StartGameHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForStartGame(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::StartGameHandler>(a_vm, a_stackId, a_functionRef);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForDeleteGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForDeleteGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForDeleteGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForNewGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForNewGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForNewGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPostLoadGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPostLoadGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPostLoadGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPostSaveGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPostSaveGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPostSaveGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForStartGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForStartGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForStartGame);
	}
}
