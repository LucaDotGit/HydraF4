#include "Plugin/Papyrus/Events.hpp"

#include "Plugin/Internal/Events/EventStructs.hpp"

#include "Plugin/Internal/Events/Handlers/Object/ActorValueChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/AnimationGraphEventHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/DestructionStageChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/DialogueTopicChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/FormDeleteHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/FormIdChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ItemAddRemoveHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectActivateHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectGrabReleaseHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectHarvestHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectHitHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectLoadUnloadHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectOpenCloseHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectResetHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/ObjectSellHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/SpellCastHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Object/TriggerEnterLeaveHandler.hpp"

namespace Plugin::Papyrus::Events::Object
{
	static bool IsRegisteredForActorValueChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ActorValueChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForActorValueChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ActorValueChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ActorValueChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForActorValueChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ActorValueChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForAnimationGraphEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::AnimationGraphEventHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForAnimationGraphEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::AnimationGraphEventArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::AnimationGraphEventHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForAnimationGraphEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::AnimationGraphEventHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForDestructionStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::DestructionStageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForDestructionStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::DestructionStageChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::DestructionStageChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForDestructionStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::DestructionStageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForDialogueTopicChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::DialogueTopicChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForDialogueTopicChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::DialogueTopicChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::DialogueTopicChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForDialogueTopicChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::DialogueTopicChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForFormDelete(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::FormDeleteHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForFormDelete(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::FormDeleteArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::FormDeleteHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForFormDelete(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::FormDeleteHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForFormIdChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::FormIdChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForFormIdChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::FormIdChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::FormIdChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForFormIdChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::FormIdChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForItemAddRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ItemAddRemoveHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForItemAddRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ItemAddRemoveArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ItemAddRemoveHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForItemAddRemove(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ItemAddRemoveHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectActivate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectActivateHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectActivate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectActivateArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectActivateHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectActivate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectActivateHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectGrabRelease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectGrabReleaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectGrabRelease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectGrabReleaseArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectGrabReleaseHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectGrabRelease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectGrabReleaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectHarvest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectHarvestHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectHarvest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectHarvestArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectHarvestHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectHarvest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectHarvestHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectHit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectHitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectHit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectHitArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectHitHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectHit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectHitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectLoadUnload(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectLoadUnloadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectLoadUnload(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectLoadUnloadArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectLoadUnloadHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectLoadUnload(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectLoadUnloadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectOpenCloseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectOpenCloseArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectOpenCloseHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectOpenCloseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectReset(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectResetHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectReset(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectResetArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectResetHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectReset(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectResetHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForObjectSell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ObjectSellHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForObjectSell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ObjectSellArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ObjectSellHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForObjectSell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ObjectSellHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForSpellCast(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::SpellCastHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForSpellCast(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::SpellCastArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::SpellCastHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForSpellCast(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::SpellCastHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForTriggerEnterLeave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::TriggerEnterLeaveHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForTriggerEnterLeave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::TriggerEnterLeaveArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::TriggerEnterLeaveHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForTriggerEnterLeave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::TriggerEnterLeaveHandler>(a_vm, a_stackId, a_functionRef);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForActorValueChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForActorValueChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForActorValueChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForAnimationGraphEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForAnimationGraphEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForAnimationGraphEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForDestructionStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForDestructionStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForDestructionStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForDialogueTopicChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForDialogueTopicChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForDialogueTopicChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForFormDelete);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForFormDelete);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForFormDelete);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForFormIdChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForFormIdChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForFormIdChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForItemAddRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForItemAddRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForItemAddRemove);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectActivate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectActivate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectActivate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectGrabRelease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectGrabRelease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectGrabRelease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectHarvest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectHarvest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectHarvest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectHit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectHit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectHit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectLoadUnload);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectLoadUnload);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectLoadUnload);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectReset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectReset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectReset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForObjectSell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForObjectSell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForObjectSell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForSpellCast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForSpellCast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForSpellCast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForTriggerEnterLeave);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForTriggerEnterLeave);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForTriggerEnterLeave);
	}
}
