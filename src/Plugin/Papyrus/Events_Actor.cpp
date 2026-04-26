#include "Plugin/Papyrus/Events.hpp"

#include "Plugin/Internal/Events/EventStructs.hpp"

#include "Plugin/Internal/Events/Handlers/Actor/AIPackageChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/ActiveEffectApplyRemoveHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/ActorDeathHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/CombatStateChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/FurnitureEnterExitHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/ItemEquipUnequipHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/LifeStateChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/LimbCrippleHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Actor/LocationEnterExitHandler.hpp"

namespace Plugin::Papyrus::Events::Actor
{
	static bool IsRegisteredForActiveEffectApplyRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ActiveEffectApplyRemoveHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForActiveEffectApplyRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ActiveEffectApplyRemoveArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ActiveEffectApplyRemoveHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForActiveEffectApplyRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ActiveEffectApplyRemoveHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForActorDeath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ActorDeathHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForActorDeath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ActorDeathArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ActorDeathHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForActorDeath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ActorDeathHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForAIPackageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::AIPackageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForAIPackageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::AIPackageChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::AIPackageChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForAIPackageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::AIPackageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForCombatStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::CombatStateChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForCombatStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::CombatStateChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::CombatStateChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForCombatStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::CombatStateChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForFurnitureEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::FurnitureEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForFurnitureEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::FurnitureEnterExitArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::FurnitureEnterExitHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForFurnitureEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::FurnitureEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForItemEquipUnequip(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ItemEquipUnequipHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForItemEquipUnequip(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ItemEquipUnequipArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ItemEquipUnequipHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForItemEquipUnequip(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ItemEquipUnequipHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLifeStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LifeStateChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLifeStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LifeStateChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LifeStateChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLifeStateChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LifeStateChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLimbCripple(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LimbCrippleHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLimbCripple(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LimbCrippleArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LimbCrippleHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLimbCripple(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LimbCrippleHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLocationEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LocationEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLocationEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LocationEnterExitArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LocationEnterExitHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLocationEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LocationEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForActiveEffectApplyRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForActiveEffectApplyRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForActiveEffectApplyRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForActorDeath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForActorDeath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForActorDeath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForAIPackageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForAIPackageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForAIPackageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForCombatStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForCombatStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForCombatStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForFurnitureEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForFurnitureEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForFurnitureEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForItemEquipUnequip);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForItemEquipUnequip);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForItemEquipUnequip);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLifeStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLifeStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLifeStateChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLimbCripple);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLimbCripple);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLimbCripple);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLocationEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLocationEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLocationEnterExit);
	}
}
