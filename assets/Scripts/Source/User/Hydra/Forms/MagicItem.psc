Scriptname Hydra:Forms:MagicItem Const Hidden Native

;/
	Provides shared data for potions, spells and enchantments.
/;

int Function GetEffectCount(Form akMagicItem) Global Native

MagicEffect Function GetNthEffect(Form akMagicItem, int aiIndex) Global Native
bool Function SetNthEffect(Form akMagicItem, int aiIndex, MagicEffect akEffect) Global Native

float Function GetNthEffectMagnitude(Form akMagicItem, int aiIndex) Global Native
bool Function SetNthEffectMagnitude(Form akMagicItem, int aiIndex, float afMagnitude) Global Native

int Function GetNthEffectArea(Form akMagicItem, int aiIndex) Global Native
bool Function SetNthEffectArea(Form akMagicItem, int aiIndex, int aiArea) Global Native

int Function GetNthEffectDuration(Form akMagicItem, int aiIndex) Global Native
bool Function SetNthEffectDuration(Form akMagicItem, int aiIndex, int aiDuration) Global Native


int Function GetType_Spell() Global Native
int Function GetType_Disease() Global Native
int Function GetType_Power() Global Native
int Function GetType_LesserPower() Global Native
int Function GetType_Ability() Global Native
int Function GetType_Poison() Global Native
int Function GetType_Enchantment() Global Native
int Function GetType_Potion() Global Native
int Function GetType_Alchemy() Global Native
int Function GetType_Ingredient() Global Native
int Function GetType_Addiction() Global Native

int Function GetCastingType_ConstantEffect() Global Native
int Function GetCastingType_FireAndForget() Global Native
int Function GetCastingType_Concentration() Global Native

int Function GetDeliveryType_Self() Global Native
int Function GetDeliveryType_Touch() Global Native
int Function GetDeliveryType_Aimed() Global Native
int Function GetDeliveryType_TargetActor() Global Native
int Function GetDeliveryType_TargetLocation() Global Native


bool Function HasFlag(Form akMagicItem, int aiFlag) Global Native
int Function GetFlags(Form akMagicItem) Global Native
Function SetFlags(Form akMagicItem, int aiFlags) Global Native
Function SetFlag(Form akMagicItem, int aiFlag, bool abSet) Global Native
