Scriptname Hydra:Forms:MagicEffect Const Hidden Native

;/
	Provides functions for magic effects.

	Notes:
	- For a list of all spell (magic effect), cast and target (delivery) types, see:
	  https://github.com/powerof3/CommonLibSSE/blob/e3626d228d60b92a82410accd475bffdd7245653/include/RE/M/MagicSystem.h#L10
/;

int Function GetArchetype(MagicEffect akMagicEffect) Global Native
Function SetArchetype(MagicEffect akMagicEffect, int aiValue) Global Native

int Function GetCastingType(MagicEffect akMagicEffect) Global Native
Function SetCastingType(MagicEffect akMagicEffect, int aiValue) Global Native

int Function GetDeliveryType(MagicEffect akMagicEffect) Global Native
Function SetDeliveryType(MagicEffect akMagicEffect, int aiValue) Global Native

ActorValue Function GetPrimaryValue(MagicEffect akMagicEffect) Global Native
Function SetPrimaryValue(MagicEffect akMagicEffect, ActorValue akValue) Global Native

ActorValue Function GetSecondaryValue(MagicEffect akMagicEffect) Global Native
Function SetSecondaryValue(MagicEffect akMagicEffect, ActorValue akValue) Global Native

float Function GetSecondaryValueWeight(MagicEffect akMagicEffect) Global Native
Function SetSecondaryValueWeight(MagicEffect akMagicEffect, float afValue) Global Native

ActorValue Function GetResistValue(MagicEffect akMagicEffect) Global Native
Function SetResistValue(MagicEffect akMagicEffect, ActorValue akValue) Global Native

Perk Function GetPerk(MagicEffect akMagicEffect) Global Native
Function SetPerk(MagicEffect akMagicEffect, Perk akValue) Global Native

float Function GetTaperCurve(MagicEffect akMagicEffect) Global Native
Function SetTaperCurve(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetTaperWeight(MagicEffect akMagicEffect) Global Native
Function SetTaperWeight(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetTaperDuration(MagicEffect akMagicEffect) Global Native
Function SetTaperDuration(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetBaseCost(MagicEffect akMagicEffect) Global Native
Function SetBaseCost(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetSkillUsageMult(MagicEffect akMagicEffect) Global Native
Function SetSkillUsageMult(MagicEffect akMagicEffect, float afValue) Global Native

VisualEffect Function GetHitVisuals(MagicEffect akMagicEffect) Global Native
Function SetHitVisuals(MagicEffect akMagicEffect, VisualEffect akValue) Global Native

VisualEffect Function GetEnchantVisuals(MagicEffect akMagicEffect) Global Native
Function SetEnchantVisuals(MagicEffect akMagicEffect, VisualEffect akValue) Global Native

ArtObject Function GetCastingArt(MagicEffect akMagicEffect) Global Native
Function SetCastingArt(MagicEffect akMagicEffect, ArtObject akValue) Global Native

Light Function GetCastingLight(MagicEffect akMagicEffect) Global Native
Function SetCastingLight(MagicEffect akMagicEffect, Light akValue) Global Native

ArtObject Function GetHitArt(MagicEffect akMagicEffect) Global Native
Function SetHitArt(MagicEffect akMagicEffect, ArtObject akValue) Global Native

EffectShader Function GetHitShader(MagicEffect akMagicEffect) Global Native
Function SetHitShader(MagicEffect akMagicEffect, EffectShader akValue) Global Native

ArtObject Function GetEnchantArt(MagicEffect akMagicEffect) Global Native
Function SetEnchantArt(MagicEffect akMagicEffect, ArtObject akValue) Global Native

EffectShader Function GetEnchantShader(MagicEffect akMagicEffect) Global Native
Function SetEnchantShader(MagicEffect akMagicEffect, EffectShader akValue) Global Native

Projectile Function GetProjectile(MagicEffect akMagicEffect) Global Native
Function SetProjectile(MagicEffect akMagicEffect, Projectile akValue) Global Native

ImpactDataSet Function GetImpactDataSet(MagicEffect akMagicEffect) Global Native
Function SetImpactDataSet(MagicEffect akMagicEffect, ImpactDataSet akValue) Global Native

Explosion Function GetExplosion(MagicEffect akMagicEffect) Global Native
Function SetExplosion(MagicEffect akMagicEffect, Explosion akValue) Global Native

ImageSpaceModifier Function GetImageSpaceMod(MagicEffect akMagicEffect) Global Native
Function SetImageSpaceMod(MagicEffect akMagicEffect, ImageSpaceModifier akValue) Global Native

int Function GetSpellArea(MagicEffect akMagicEffect) Global Native
Function SetSpellArea(MagicEffect akMagicEffect, int aiValue) Global Native

float Function GetSpellChargeTime(MagicEffect akMagicEffect) Global Native
Function SetSpellChargeTime(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetAIScore(MagicEffect akMagicEffect) Global Native
Function SetAIScore(MagicEffect akMagicEffect, float afValue) Global Native

float Function GetAIDelayTime(MagicEffect akMagicEffect) Global Native
Function SetAIDelayTime(MagicEffect akMagicEffect, float afValue) Global Native

Spell Function GetEquipAbility(MagicEffect akMagicEffect) Global Native
Function SetEquipAbility(MagicEffect akMagicEffect, Spell akValue) Global Native

string Function GetDescription(MagicEffect akMagicEffect) Global Native
Function SetDescription(MagicEffect akMagicEffect, string acsValue) Global Native


bool Function HasFlag(MagicEffect akMagicEffect, int aiFlag) Global Native
int Function GetFlags(MagicEffect akMagicEffect) Global Native
Function SetFlags(MagicEffect akMagicEffect, int aiFlags) Global Native
Function SetFlag(MagicEffect akMagicEffect, int aiFlag, bool abSet) Global Native

;/
	Magic Effect Flags:
/;

int Function GetFlag_IsHostile() Global Native
int Function GetFlag_Recover() Global Native
int Function GetFlag_IsDetrimental() Global Native
int Function GetFlag_SnapToNavMesh() Global Native
int Function GetFlag_HasNoHitEvent() Global Native
int Function GetFlag_DispelWithKeywords() Global Native
int Function GetFlag_HasNoDuration() Global Native
int Function GetFlag_HasNoMagnitude() Global Native
int Function GetFlag_HasNoArea() Global Native
int Function GetFlag_FXPersist() Global Native
int Function GetFlag_HasGoryVisuals() Global Native
int Function GetFlag_IsHiddenInUI() Global Native
int Function GetFlag_HasNoRecast() Global Native
int Function GetFlag_PowerAffectMagnitude() Global Native
int Function GetFlag_PowerAffectDuration() Global Native
int Function GetFlag_IsPainless() Global Native
int Function GetFlag_HasNoHitEffect() Global Native
int Function GetFlag_HasNoDeathDispel() Global Native
