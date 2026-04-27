Scriptname Hydra:Forms:Enchantment Const Hidden Native

;/
	Provides functions for enchantments.

	Notes:
	- For a list of all spell (enchantment), cast and delivery types, see the `Hydra:MagicItem` script.
/;

Import Hydra:Forms:MagicItem

int Function GetType(Enchantment akEnchantment) Global Native
Function SetType(Enchantment akEnchantment, int aiValue) Global Native

int Function GetCastingType(Enchantment akEnchantment) Global Native
Function SetCastingType(Enchantment akEnchantment, int aiValue) Global Native

int Function GetDeliveryType(Enchantment akEnchantment) Global Native
Function SetDeliveryType(Enchantment akEnchantment, int aiValue) Global Native

float Function GetChargeTime(Enchantment akEnchantment) Global Native
Function SetChargeTime(Enchantment akEnchantment, float afValue) Global Native

Enchantment Function GetBaseEnchantment(Enchantment akEnchantment) Global Native
Function SetBaseEnchantment(Enchantment akEnchantment, Enchantment akValue) Global Native

FormList Function GetWornRestrictionList(Enchantment akEnchantment) Global Native
Function SetWornRestrictionList(Enchantment akEnchantment, FormList akValue) Global Native

int Function GetEffectCount(Enchantment akEnchantment) Global
	return Hydra:Forms:MagicItem.GetEffectCount(akEnchantment)
EndFunction

MagicEffect Function GetNthEffect(Enchantment akEnchantment, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffect(akEnchantment, aiIndex)
EndFunction
bool Function SetNthEffect(Enchantment akEnchantment, int aiIndex, MagicEffect akEffect) Global
	return Hydra:Forms:MagicItem.SetNthEffect(akEnchantment, aiIndex, akEffect)
EndFunction

float Function GetNthEffectMagnitude(Enchantment akEnchantment, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectMagnitude(akEnchantment, aiIndex)
EndFunction
bool Function SetNthEffectMagnitude(Enchantment akEnchantment, int aiIndex, float afMagnitude) Global
	return Hydra:Forms:MagicItem.SetNthEffectMagnitude(akEnchantment, aiIndex, afMagnitude)
EndFunction

int Function GetNthEffectArea(Enchantment akEnchantment, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectArea(akEnchantment, aiIndex)
EndFunction
bool Function SetNthEffectArea(Enchantment akEnchantment, int aiIndex, int aiArea) Global
	return Hydra:Forms:MagicItem.SetNthEffectArea(akEnchantment, aiIndex, aiArea)
EndFunction

int Function GetNthEffectDuration(Enchantment akEnchantment, int aiIndex) Global
	return Hydra:Forms:MagicItem.GetNthEffectDuration(akEnchantment, aiIndex)
EndFunction
bool Function SetNthEffectDuration(Enchantment akEnchantment, int aiIndex, int aiDuration) Global
	return Hydra:Forms:MagicItem.SetNthEffectDuration(akEnchantment, aiIndex, aiDuration)
EndFunction


bool Function HasFlag(Enchantment akEnchantment, int aiFlag) Global
	return Hydra:Forms:MagicItem.HasFlag(akEnchantment, aiFlag)
EndFunction
int Function GetFlags(Enchantment akEnchantment) Global
	return Hydra:Forms:MagicItem.GetFlags(akEnchantment)
EndFunction
Function SetFlags(Enchantment akEnchantment, int aiFlags) Global
	Hydra:Forms:MagicItem.SetFlags(akEnchantment, aiFlags)
EndFunction
Function SetFlag(Enchantment akEnchantment, int aiFlag, bool abSet) Global
	Hydra:Forms:MagicItem.SetFlag(akEnchantment, aiFlag, abSet)
EndFunction

;/
	Enchantment Flags:
/;

int Function GetFlag_HasManualCostCalc() Global Native
int Function GetFlag_IsFood() Global Native
int Function GetFlag_HasExtendedDuration() Global Native
