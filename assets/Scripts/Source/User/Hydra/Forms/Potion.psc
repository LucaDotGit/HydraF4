Scriptname Hydra:Forms:Potion Const Hidden Native

;/
	Provides functions for potions.
/;

Import Hydra:Forms:MagicItem

Sound Function GetConsumeSound(Potion akPotion) Global Native
Function SetConsumeSound(Potion akPotion, Sound akValue) Global Native

Spell Function GetAddiction(Potion akPotion) Global Native
Function SetAddiction(Potion akPotion, Spell akValue) Global Native

float Function GetAddictionChance(Potion akPotion) Global Native
Function SetAddictionChance(Potion akPotion, float afValue) Global Native

string Function GetAddictionName(Potion akPotion) Global Native
Function SetAddictionName(Potion akPotion, string acsValue) Global Native

int Function GetEffectCount(Potion akPotion) Global
	return Hydra:Forms:MagicItem.GetEffectCount(akPotion)
EndFunction

MagicEffect Function GetNthEffect(Potion akPotion, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffect(akPotion, aiIndex)
EndFunction
bool Function SetNthEffect(Potion akPotion, int aiIndex, MagicEffect akEffect) Global
	return Hydra:Forms:MagicItem.SetNthEffect(akPotion, aiIndex, akEffect)
EndFunction

float Function GetNthEffectMagnitude(Potion akPotion, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectMagnitude(akPotion, aiIndex)
EndFunction
bool Function SetNthEffectMagnitude(Potion akPotion, int aiIndex, float afMagnitude) Global
	return Hydra:Forms:MagicItem.SetNthEffectMagnitude(akPotion, aiIndex, afMagnitude)
EndFunction

int Function GetNthEffectArea(Potion akPotion, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectArea(akPotion, aiIndex)
EndFunction
bool Function SetNthEffectArea(Potion akPotion, int aiIndex, int aiArea) Global
	return Hydra:Forms:MagicItem.SetNthEffectArea(akPotion, aiIndex, aiArea)
EndFunction

int Function GetNthEffectDuration(Potion akPotion, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectDuration(akPotion, aiIndex)
EndFunction
bool Function SetNthEffectDuration(Potion akPotion, int aiIndex, int aiDuration) Global
	return Hydra:Forms:MagicItem.SetNthEffectDuration(akPotion, aiIndex, aiDuration)
EndFunction


bool Function HasFlag(Potion akPotion, int aiFlag) Global
	return Hydra:Forms:MagicItem.HasFlag(akPotion, aiFlag)
EndFunction
int Function GetFlags(Potion akPotion) Global
	return Hydra:Forms:MagicItem.GetFlags(akPotion)
EndFunction
Function SetFlags(Potion akPotion, int aiFlags) Global
	Hydra:Forms:MagicItem.SetFlags(akPotion, aiFlags)
EndFunction
Function SetFlag(Potion akPotion, int aiFlag, bool abSet) Global
	Hydra:Forms:MagicItem.SetFlag(akPotion, aiFlag, abSet)
EndFunction

;/
	Potion Flags:
/;

int Function GetFlag_HasManualCostCalc() Global Native
int Function GetFlag_IsFood() Global Native
int Function GetFlag_IsPoison() Global Native
int Function GetFlag_IsMedicine() Global Native
