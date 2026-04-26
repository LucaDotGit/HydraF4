#include "Plugin/Papyrus/Events.hpp"

#include "Plugin/Internal/Events/EventStructs.hpp"

#include "Plugin/Internal/Events/Handlers/Game/BookReadHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/ButtonUpDownHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/CellAttachDetachHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/CellEnterExitHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/CellLoadHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/CrosshairRefChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/DialogueTargetChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/DifficultyChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/HudColorUpdateHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/LevelIncreaseHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/LocationLoadHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/LockPickHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/MenuModeEnterExitHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/MenuOpenCloseHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/MiscStatChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/PerkEntryRunHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/PerkPointIncreaseHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/PipBoyLightChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/PowerArmorLightChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/QuestObjectiveChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/QuestStageChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/QuestStartStopHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/SceneActionChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/ScenePhaseChangeHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/SceneStartStopHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/SleepStartStopHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/TerminalHackHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/TerminalMenuItemRunHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/TutorialTriggerHandler.hpp"
#include "Plugin/Internal/Events/Handlers/Game/WaitStartStopHandler.hpp"

namespace Plugin::Papyrus::Events::Game
{
	static bool IsRegisteredForBookRead(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::BookReadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForBookRead(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::BookReadArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::BookReadHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForBookRead(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::BookReadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForButtonUpDown(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ButtonUpDownHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForButtonUpDown(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ButtonUpDownArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ButtonUpDownHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForButtonUpDown(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ButtonUpDownHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForCellAttachDetach(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::CellAttachDetachHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForCellAttachDetach(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::CellAttachDetachArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::CellAttachDetachHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForCellAttachDetach(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::CellAttachDetachHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForCellEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::CellEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForCellEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::CellEnterExitArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::CellEnterExitHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForCellEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::CellEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForCellLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::CellLoadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForCellLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::CellLoadArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::CellLoadHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForCellLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::CellLoadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForCrosshairRefChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::CrosshairRefChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForCrosshairRefChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::CrosshairRefChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::CrosshairRefChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForCrosshairRefChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::CrosshairRefChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForDialogueTargetChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::DialogueTargetChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForDialogueTargetChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::DialogueTargetChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::DialogueTargetChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForDialogueTargetChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::DialogueTargetChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForDifficultyChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::DifficultyChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForDifficultyChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::DifficultyChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::DifficultyChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForDifficultyChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::DifficultyChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForHudColorUpdate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::HudColorUpdateHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForHudColorUpdate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::HudColorUpdateArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::HudColorUpdateHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForHudColorUpdate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::HudColorUpdateHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLevelIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LevelIncreaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLevelIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LevelIncreaseArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LevelIncreaseHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLevelIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LevelIncreaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLocationLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LocationLoadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLocationLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LocationLoadArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LocationLoadHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLocationLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LocationLoadHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForLockPick(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::LockPickHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForLockPick(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::LockPickArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::LockPickHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForLockPick(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::LockPickHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForMenuModeEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::MenuModeEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForMenuModeEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::MenuModeEnterExitArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::MenuModeEnterExitHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForMenuModeEnterExit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::MenuModeEnterExitHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForMenuOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::MenuOpenCloseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForMenuOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::MenuOpenCloseArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::MenuOpenCloseHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForMenuOpenClose(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::MenuOpenCloseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForMiscStatChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::MiscStatChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForMiscStatChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::MiscStatChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::MiscStatChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForMiscStatChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::MiscStatChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPerkEntryRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PerkEntryRunHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPerkEntryRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PerkEntryRunArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PerkEntryRunHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPerkEntryRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PerkEntryRunHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPerkPointIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PerkPointIncreaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPerkPointIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PerkPointIncreaseArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PerkPointIncreaseHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPerkPointIncrease(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PerkPointIncreaseHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPipBoyLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PipBoyLightChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPipBoyLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PipBoyLightChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PipBoyLightChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPipBoyLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PipBoyLightChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForPowerArmorLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::PowerArmorLightChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForPowerArmorLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::PowerArmorLightChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::PowerArmorLightChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForPowerArmorLightChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::PowerArmorLightChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForQuestObjectiveChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::QuestObjectiveChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForQuestObjectiveChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::QuestObjectiveChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::QuestObjectiveChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForQuestObjectiveChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::QuestObjectiveChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForQuestStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::QuestStageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForQuestStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::QuestStageChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::QuestStageChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForQuestStageChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::QuestStageChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForQuestStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::QuestStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForQuestStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::QuestStartStopArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::QuestStartStopHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForQuestStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::QuestStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForSceneActionChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::SceneActionChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForSceneActionChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::SceneActionChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::SceneActionChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForSceneActionChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::SceneActionChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForScenePhaseChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::ScenePhaseChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForScenePhaseChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::ScenePhaseChangeArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::ScenePhaseChangeHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForScenePhaseChange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::ScenePhaseChangeHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForSceneStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::SceneStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForSceneStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::SceneStartStopArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::SceneStartStopHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForSceneStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::SceneStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForSleepStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::SleepStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForSleepStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::SleepStartStopArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::SleepStartStopHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForSleepStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::SleepStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForTerminalHack(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::TerminalHackHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForTerminalHack(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::TerminalHackArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::TerminalHackHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForTerminalHack(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::TerminalHackHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForTerminalMenuItemRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::TerminalMenuItemRunHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForTerminalMenuItemRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::TerminalMenuItemRunArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::TerminalMenuItemRunHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForTerminalMenuItemRun(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::TerminalMenuItemRunHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForTutorialTrigger(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::TutorialTriggerHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForTutorialTrigger(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::TutorialTriggerArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::TutorialTriggerHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForTutorialTrigger(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::TutorialTriggerHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool IsRegisteredForWaitStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::IsRegistered<::Plugin::Internal::Events::WaitStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	static bool RegisterForWaitStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Events::WaitStartStopArgs a_args,
		bool a_persistent)
	{
		return Impl::Register<::Plugin::Internal::Events::WaitStartStopHandler>(a_vm, a_stackId, a_functionRef, a_args, a_persistent);
	}

	static bool UnregisterForWaitStartStop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		return Impl::Unregister<::Plugin::Internal::Events::WaitStartStopHandler>(a_vm, a_stackId, a_functionRef);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForBookRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForBookRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForBookRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForButtonUpDown);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForButtonUpDown);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForButtonUpDown);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForCellAttachDetach);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForCellAttachDetach);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForCellAttachDetach);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForCellEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForCellEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForCellEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForCellLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForCellLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForCellLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForCrosshairRefChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForCrosshairRefChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForCrosshairRefChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForDialogueTargetChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForDialogueTargetChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForDialogueTargetChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForDifficultyChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForDifficultyChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForDifficultyChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForHudColorUpdate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForHudColorUpdate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForHudColorUpdate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLevelIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLevelIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLevelIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLocationLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLocationLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLocationLoad);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForLockPick);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForLockPick);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForLockPick);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForMenuModeEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForMenuModeEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForMenuModeEnterExit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForMenuOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForMenuOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForMenuOpenClose);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForMiscStatChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForMiscStatChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForMiscStatChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPerkEntryRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPerkEntryRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPerkEntryRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPerkPointIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPerkPointIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPerkPointIncrease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPipBoyLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPipBoyLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPipBoyLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForPowerArmorLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForPowerArmorLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForPowerArmorLightChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForQuestObjectiveChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForQuestObjectiveChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForQuestObjectiveChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForQuestStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForQuestStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForQuestStageChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForQuestStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForQuestStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForQuestStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForSceneActionChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForSceneActionChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForSceneActionChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForScenePhaseChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForScenePhaseChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForScenePhaseChange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForSceneStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForSceneStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForSceneStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForSleepStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForSleepStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForSleepStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForTerminalHack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForTerminalHack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForTerminalHack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForTerminalMenuItemRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForTerminalMenuItemRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForTerminalMenuItemRun);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForTutorialTrigger);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForTutorialTrigger);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForTutorialTrigger);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForWaitStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RegisterForWaitStartStop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnregisterForWaitStartStop);
	}
}
