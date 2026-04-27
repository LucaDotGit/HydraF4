Scriptname Hydra:Forms:ActiveMagicEffect Const Hidden Native

;/
	Provides functions for active magic effects.
/;

int Function GetId(ActiveMagicEffect akActiveEffect) Global Native

Form Function GetSourceObject(ActiveMagicEffect akActiveEffect) Global Native
Actor Function GetSourceActor(ActiveMagicEffect akActiveEffect) Global Native
Actor Function GetTargetActor(ActiveMagicEffect akActiveEffect) Global Native

MagicEffect Function GetBaseEffect(ActiveMagicEffect akActiveEffect) Global Native
Spell Function GetSpell(ActiveMagicEffect akActiveEffect) Global Native

float Function GetElapsedTime(ActiveMagicEffect akActiveEffect) Global Native
float Function GetDuration(ActiveMagicEffect akActiveEffect) Global Native
float Function GetMagnitude(ActiveMagicEffect akActiveEffect) Global Native


bool Function HasFlag(ActiveMagicEffect akActiveEffect, int aiFlag) Global Native
int Function GetFlags(ActiveMagicEffect akActiveEffect) Global Native
Function SetFlags(ActiveMagicEffect akActiveEffect, int aiFlags) Global Native
Function SetFlag(ActiveMagicEffect akActiveEffect, int aiFlag, bool abValue) Global Native

;/
	Active Effect Flags:
/;

int Function GetFlag_HasNoHitShader() Global Native
int Function GetFlag_HasNoHitEffectArt() Global Native
int Function GetFlag_HasNoInitialFlare() Global Native
int Function GetFlag_IsApplyingHitEffects() Global Native
int Function GetFlag_IsApplyingSounds() Global Native
int Function GetFlag_HasConditions() Global Native
int Function GetFlag_Recover() Global Native
int Function GetFlag_IsDualCasted() Global Native
int Function GetFlag_IsInactive() Global Native
int Function GetFlag_HasAppliedEffects() Global Native
int Function GetFlag_HasRemovedEffects() Global Native
int Function GetFlag_IsDispelled() Global Native
int Function GetFlag_HasWornOff() Global Native
