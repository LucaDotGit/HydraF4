Scriptname Hydra:Forms:Actor Const Hidden Native

;/
	Provides functions for actors.
/;

ActorBase Function GetActorBase(Actor akActor) Global Native
Function SetActorBase(Actor akActor, ActorBase akValue) Global Native

bool Function ContainsEffect(Actor akActor, MagicEffect akEffect) Global Native
MagicEffect[] Function GetEffects(Actor akActor) Global Native

bool Function ContainsActiveEffect(Actor akActor, ActiveMagicEffect akActiveEffect) Global Native
ActiveMagicEffect[] Function GetActiveEffects(Actor akActor) Global Native

bool Function ContainsSpell(Actor akActor, Spell akSpell) Global Native
Spell[] Function GetSpells(Actor akActor) Global Native

bool Function CanUseIdle(Actor akActor, Idle akIdle) Global Native
Idle Function GetCurrentIdle(Actor akActor) Global Native
Idle Function GetCurrentFurnitureIdle(Actor akActor) Global Native
Idle Function GetLastPlayedIdle(Actor akActor) Global Native

int Function GetPerkRank(Actor akActor, Perk akPerk) Global Native

;/
	3D Update Flags:
	< 0		All
	0x000	None
	0x001	Model
	0x002	Skin
	0x004	Head
	0x008	Face
	0x010	Scale
	0x020	Skeleton
	0x040	Init Default
	0x080	Sky Cell Skin
	0x100	Havok
	0x200	Do Not Add Outfit
	0x400	Keep Head
	0x800	Dismemberment
/;
Function Update3DAsync(Actor akActor, int aiFlags = -1) Global Native

;/
	Actor States:
	00	Normal
	01	Wants To Sit
	02	Waiting For Sit Anim
	03	Sitting
	04	Wants To Stand
	05	Wants To Sleep
	06	Waiting For Sleep Anim
	07	Sleeping
	08	Wants To Wake Up
/;
int Function GetSitSleepState(Actor akActor) Global Native

;/
	Fly States:
	00	None
	01	Take Off
	02	Cruising
	03	Hovering
	04	Landing
	05	Perching
	06	Action
/;
int Function GetFlyState(Actor akActor) Global Native

;/
	Life States:
	00	Alive
	01	Dying
	02	Dead
	03	Unconscious
	04	Reanimate
	05	Recycle
	06	Restrained
	07	Essential Down
	08	Bleedout
/;
int Function GetLifeState(Actor akActor) Global Native

;/
	Combat States:
	00	None
	01	Draw
	02	Swing
	03	Hit
	04	Next Attack
	05	Follow Through
	06	Bash
	08	Bow Draw
	09	Bow Attached
	10	Bow Drawn
	11	Bow Releasing
	12	Bow Released
	13	Bow Next Attack
	14	Bow Follow Through
	15	Fire
	16	Firing
	17	Fired
/;
int Function GetAttackState(Actor akActor) Global Native

;/
	Knock States:
	00	Normal
	01	Explosion
	02	Explosion Lead In
	03	Out
	04	Out Lead In
	05	Queued
	06	Get Up
	07	Down
	08	Wait For Task Queue
/;
int Function GetKnockState(Actor akActor) Global Native

;/
	Weapon States:
	00	Sheathed
	01	Wants To Draw
	02	Drawing
	03	Drawn
	04	Wants To Sheathe
	05	Sheathing
/;
int Function GetWeaponState(Actor akActor) Global Native

;/
	Stance States:
	00	None
	01	Small
	02	Large
/;
int Function GetRecoilState(Actor akActor) Global Native

;/
	Stance States:
	00	Normal
	01	Sneaking
	02	Cover Very Low
	03	Cover Low
	04	Cover Medium
	05	Cover High
/;
int Function GetStanceState(Actor akActor) Global Native

;/
	Gun States:
	00	Drawn
	01	Relaxed
	02	Blocked
	03	Alert
	04	Reloading
	05	Throwing
	06	Sighted
	07	Firing
	08	Sighted Firing
/;
int Function GetGunState(Actor akActor) Global Native

;/
	Interaction States:
	00	Not Interacting
	01	Waiting To Interact
	02	Interacting
	03	Waiting To Stop Interacting
/;
int Function GetInteractionState(Actor akActor) Global Native
