Scriptname Hydra:Forms:ArmorAddon Const Hidden Native

;/
	Provides functions for armor addons.
/;

Import Hydra:Vectors3

int Function GetMalePriority(ArmorAddon akArmorAddon) Global Native
Function SetMalePriority(ArmorAddon akArmorAddon, int aiValue) Global Native

int Function GetFemalePriority(ArmorAddon akArmorAddon) Global Native
Function SetFemalePriority(ArmorAddon akArmorAddon, int aiValue) Global Native

int Function GetDetectionSoundValue(ArmorAddon akArmorAddon) Global Native
Function SetDetectionSoundValue(ArmorAddon akArmorAddon, int aiValue) Global Native

float Function GetWeaponAdjustment(ArmorAddon akArmorAddon) Global Native
Function SetWeaponAdjustment(ArmorAddon akArmorAddon, float afValue) Global Native

FormList Function GetMaleTextureSwapList(ArmorAddon akArmorAddon) Global Native
Function SetMaleTextureSwapList(ArmorAddon akArmorAddon, FormList akValue) Global Native

FormList Function GetFemaleTextureSwapList(ArmorAddon akArmorAddon) Global Native
Function SetFemaleTextureSwapList(ArmorAddon akArmorAddon, FormList akValue) Global Native

Race[] Function GetAdditionalRaces(ArmorAddon akArmorAddon) Global Native
Function SetAdditionalRaces(ArmorAddon akArmorAddon, Race[] akValues) Global Native

FootstepSet Function GetFootstepSet(ArmorAddon akArmorAddon) Global Native
Function SetFootstepSet(ArmorAddon akArmorAddon, FootstepSet akValue) Global Native

ArtObject Function GetArtObject(ArmorAddon akArmorAddon) Global Native
Function SetArtObject(ArmorAddon akArmorAddon, ArtObject akValue) Global Native

Vector3 Function GetMaleBoneScaleModifierByName(ArmorAddon akArmorAddon, string asName) Global Native
bool Function SetMaleBoneScaleModifierByName(ArmorAddon akArmorAddon, string asName, Vector3 akValue) Global Native

Vector3 Function GetFemaleBoneScaleModifierByName(ArmorAddon akArmorAddon, string asName) Global Native
bool Function SetFemaleBoneScaleModifierByName(ArmorAddon akArmorAddon, string asName, Vector3 akValue) Global Native
