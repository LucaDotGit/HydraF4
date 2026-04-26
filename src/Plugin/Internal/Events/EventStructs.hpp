#pragma once

namespace Plugin::Internal::Events
{
	inline static constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Events");

	using UserEventArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "UserEventArgs">;
	using UserEventParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "UserEventParams">;

	using DeleteGameArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "DeleteGameArgs">;
	using DeleteGameParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "DeleteGameParams">;

	using NewGameArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "NewGameArgs">;
	using NewGameParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "NewGameParams">;

	using PostLoadGameArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PostLoadGameArgs">;
	using PostLoadGameParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PostLoadGameParams">;

	using PostSaveGameArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PostSaveGameArgs">;
	using PostSaveGameParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PostSaveGameParams">;

	using StartGameArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "StartGameArgs">;
	using StartGameParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "StartGameParams">;

	using AIPackageChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "AIPackageChangeArgs">;
	using AIPackageChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "AIPackageChangeParams">;

	using ActiveEffectApplyRemoveArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActiveEffectApplyRemoveArgs">;
	using ActiveEffectApplyRemoveParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActiveEffectApplyRemoveParams">;

	using ActorDeathArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActorDeathArgs">;
	using ActorDeathParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActorDeathParams">;

	using CombatStateChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "CombatStateChangeArgs">;
	using CombatStateChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "CombatStateChangeParams">;

	using FurnitureEnterExitArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "FurnitureEnterExitArgs">;
	using FurnitureEnterExitParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "FurnitureEnterExitParams">;

	using ItemEquipUnequipArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ItemEquipUnequipArgs">;
	using ItemEquipUnequipParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ItemEquipUnequipParams">;

	using LifeStateChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LifeStateChangeArgs">;
	using LifeStateChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LifeStateChangeParams">;

	using LimbCrippleArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LimbCrippleArgs">;
	using LimbCrippleParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LimbCrippleParams">;

	using LocationEnterExitArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LocationEnterExitArgs">;
	using LocationEnterExitParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LocationEnterExitParams">;

	using ActorValueChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActorValueChangeArgs">;
	using ActorValueChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActorValueChangeParams">;

	using AnimationGraphEventArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "AnimationGraphEventArgs">;
	using AnimationGraphEventParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "AnimationGraphEventParams">;

	using DestructionStageChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "DestructionStageChangeArgs">;
	using DestructionStageChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "DestructionStageChangeParams">;

	using DialogueTopicChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "DialogueTopicChangeArgs">;
	using DialogueTopicChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "DialogueTopicChangeParams">;

	using FormDeleteArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "FormDeleteArgs">;
	using FormDeleteParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "FormDeleteParams">;

	using FormIdChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "FormIdChangeArgs">;
	using FormIdChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "FormIdChangeParams">;

	using ItemAddRemoveArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ItemAddRemoveArgs">;
	using ItemAddRemoveParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ItemAddRemoveParams">;

	using ObjectActivateArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectActivateArgs">;
	using ObjectActivateParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectActivateParams">;

	using ObjectGrabReleaseArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectGrabReleaseArgs">;
	using ObjectGrabReleaseParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectGrabReleaseParams">;

	using ObjectHarvestArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectHarvestArgs">;
	using ObjectHarvestParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectHarvestParams">;

	using ObjectHitArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectHitArgs">;
	using ObjectHitParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectHitParams">;
	using ObjectHitData = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectHitData">;

	using ObjectLoadUnloadArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectLoadUnloadArgs">;
	using ObjectLoadUnloadParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectLoadUnloadParams">;

	using ObjectOpenCloseArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectOpenCloseArgs">;
	using ObjectOpenCloseParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectOpenCloseParams">;

	using ObjectResetArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectResetArgs">;
	using ObjectResetParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectResetParams">;

	using ObjectSellArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectSellArgs">;
	using ObjectSellParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ObjectSellParams">;

	using SpellCastArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "SpellCastArgs">;
	using SpellCastParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "SpellCastParams">;

	using TriggerEnterLeaveArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "TriggerEnterLeaveArgs">;
	using TriggerEnterLeaveParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "TriggerEnterLeaveParams">;

	using CrosshairRefChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "CrosshairRefChangeArgs">;
	using CrosshairRefChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "CrosshairRefChangeParams">;

	using HudColorUpdateArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "HudColorUpdateArgs">;
	using HudColorUpdateParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "HudColorUpdateParams">;

	using MenuModeEnterExitArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "MenuModeEnterExitArgs">;
	using MenuModeEnterExitParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "MenuModeEnterExitParams">;

	using MenuOpenCloseArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "MenuOpenCloseArgs">;
	using MenuOpenCloseParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "MenuOpenCloseParams">;

	using TutorialTriggerArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "TutorialTriggerArgs">;
	using TutorialTriggerParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "TutorialTriggerParams">;

	using BookReadArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "BookReadArgs">;
	using BookReadParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "BookReadParams">;

	using ButtonUpDownArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ButtonUpDownArgs">;
	using ButtonUpDownParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ButtonUpDownParams">;

	using CellAttachDetachArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellAttachDetachArgs">;
	using CellAttachDetachParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellAttachDetachParams">;

	using CellEnterExitArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellEnterExitArgs">;
	using CellEnterExitParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellEnterExitParams">;

	using CellLoadArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellLoadArgs">;
	using CellLoadParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "CellLoadParams">;

	using DialogueTargetChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "DialogueTargetChangeArgs">;
	using DialogueTargetChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "DialogueTargetChangeParams">;

	using DifficultyChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "DifficultyChangeArgs">;
	using DifficultyChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "DifficultyChangeParams">;

	using LevelIncreaseArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LevelIncreaseArgs">;
	using LevelIncreaseParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LevelIncreaseParams">;

	using LocationLoadArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LocationLoadArgs">;
	using LocationLoadParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LocationLoadParams">;

	using LockPickArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "LockPickArgs">;
	using LockPickParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "LockPickParams">;

	using MiscStatChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "MiscStatChangeArgs">;
	using MiscStatChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "MiscStatChangeParams">;

	using PerkEntryRunArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PerkEntryRunArgs">;
	using PerkEntryRunParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PerkEntryRunParams">;

	using PerkPointIncreaseArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PerkPointIncreaseArgs">;
	using PerkPointIncreaseParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PerkPointIncreaseParams">;

	using PipBoyLightChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PipBoyLightChangeArgs">;
	using PipBoyLightChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PipBoyLightChangeParams">;

	using PowerArmorLightChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "PowerArmorLightChangeArgs">;
	using PowerArmorLightChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "PowerArmorLightChangeParams">;

	using QuestObjectiveChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestObjectiveChangeArgs">;
	using QuestObjectiveChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestObjectiveChangeParams">;

	using QuestStageChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestStageChangeArgs">;
	using QuestStageChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestStageChangeParams">;

	using QuestStartStopArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestStartStopArgs">;
	using QuestStartStopParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestStartStopParams">;

	using SceneActionChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "SceneActionChangeArgs">;
	using SceneActionChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "SceneActionChangeParams">;

	using ScenePhaseChangeArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "ScenePhaseChangeArgs">;
	using ScenePhaseChangeParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "ScenePhaseChangeParams">;

	using SceneStartStopArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "SceneStartStopArgs">;
	using SceneStartStopParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "SceneStartStopParams">;

	using SleepStartStopArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "SleepStartStopArgs">;
	using SleepStartStopParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "SleepStartStopParams">;

	using TerminalHackArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "TerminalHackArgs">;
	using TerminalHackParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "TerminalHackParams">;

	using TerminalMenuItemRunArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "TerminalMenuItemRunArgs">;
	using TerminalMenuItemRunParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "TerminalMenuItemRunParams">;

	using WaitStartStopArgs = RE::BSScript::StructWrapper<SCRIPT_NAME, "WaitStartStopArgs">;
	using WaitStartStopParams = RE::BSScript::StructWrapper<SCRIPT_NAME, "WaitStartStopParams">;
}
