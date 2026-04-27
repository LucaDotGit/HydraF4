Scriptname Hydra:Forms:Spell Const Hidden Native

;/
	Provides functions for spells.

	Notes:
	- For a list of all spell (enchantment), cast and delivery types, see the `Hydra:MagicItem` script.
/;

Import Hydra:Forms:MagicItem

int Function GetType(Spell akSpell) Global Native
Function SetType(Spell akSpell, int aiValue) Global Native

int Function GetCastingType(Spell akSpell) Global Native
Function SetCastingType(Spell akSpell, int aiValue) Global Native

int Function GetDeliveryType(Spell akSpell) Global Native
Function SetDeliveryType(Spell akSpell, int aiValue) Global Native

float Function GetRange(Spell akSpell) Global Native
Function SetRange(Spell akSpell, float afValue) Global Native

float Function GetChargeTime(Spell akSpell) Global Native
Function SetChargeTime(Spell akSpell, float afValue) Global Native

float Function GetCastDuration(Spell akSpell) Global Native
Function SetCastDuration(Spell akSpell, float afValue) Global Native

Perk Function GetCastingPerk(Spell akSpell) Global Native
Function SetCastingPerk(Spell akSpell, Perk akValue) Global Native

int Function GetEffectCount(Spell akSpell) Global
	return Hydra:Forms:MagicItem.GetEffectCount(akSpell)
EndFunction

MagicEffect Function GetNthEffect(Spell akSpell, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffect(akSpell, aiIndex)
EndFunction
bool Function SetNthEffect(Spell akSpell, int aiIndex, MagicEffect akEffect) Global
	return Hydra:Forms:MagicItem.SetNthEffect(akSpell, aiIndex, akEffect)
EndFunction

float Function GetNthEffectMagnitude(Spell akSpell, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectMagnitude(akSpell, aiIndex)
EndFunction
bool Function SetNthEffectMagnitude(Spell akSpell, int aiIndex, float afMagnitude) Global
	return Hydra:Forms:MagicItem.SetNthEffectMagnitude(akSpell, aiIndex, afMagnitude)
EndFunction

int Function GetNthEffectArea(Spell akSpell, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectArea(akSpell, aiIndex)
EndFunction
bool Function SetNthEffectArea(Spell akSpell, int aiIndex, int aiArea) Global
	return Hydra:Forms:MagicItem.SetNthEffectArea(akSpell, aiIndex, aiArea)
EndFunction

int Function GetNthEffectDuration(Spell akSpell, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectDuration(akSpell, aiIndex)
EndFunction
bool Function SetNthEffectDuration(Spell akSpell, int aiIndex, int aiDuration) Global
	return Hydra:Forms:MagicItem.SetNthEffectDuration(akSpell, aiIndex, aiDuration)
EndFunction


bool Function HasFlag(Spell akSpell, int aiFlag) Global
	return Hydra:Forms:MagicItem.HasFlag(akSpell, aiFlag)
EndFunction
int Function GetFlags(Spell akSpell) Global
	return Hydra:Forms:MagicItem.GetFlags(akSpell)
EndFunction
Function SetFlags(Spell akSpell, int aiFlags) Global
	Hydra:Forms:MagicItem.SetFlags(akSpell, aiFlags)
EndFunction
Function SetFlag(Spell akSpell, int aiFlag, bool abSet) Global
	Hydra:Forms:MagicItem.SetFlag(akSpell, aiFlag, abSet)
EndFunction

;/
	Spell Flags:
/;

int Function GetFlag_HasManualCostCalc() Global Native
int Function GetFlag_IsPlayerStartSpell() Global Native
int Function GetFlag_IsInstantCast() Global Native
int Function GetFlag_AreaEffectIgnoreLos() Global Native
int Function GetFlag_IgnoreResist() Global Native
int Function GetFlag_HasNoAbsorbReflect() Global Native
int Function GetFlag_HasNoDualCastMods() Global Native
