Scriptname Hydra:Forms:Explosion Const Hidden Native

;/
	Provides functions for explosions.

	Sound Levels:
	00	Loud
	01	Normal
	02	Silent
	03	Very Loud
	04	Quiet

	Stagger Magnitudes:
	00	None
	01	Small
	02	Medium
	03	Large
	04	Extra Large
/;

Import Hydra:Vectors3

Light Function GetLight(Explosion akExplosion) Global Native
Function SetLight(Explosion akExplosion, Light akLight) Global Native

Sound Function GetSound01(Explosion akExplosion) Global Native
Function SetSound01(Explosion akExplosion, Sound akSound) Global Native

Sound Function GetSound02(Explosion akExplosion) Global Native
Function SetSound02(Explosion akExplosion, Sound akSound) Global Native

ImpactDataSet Function GetImpactDataSet(Explosion akExplosion) Global Native
Function SetImpactDataSet(Explosion akExplosion, ImpactDataSet akImpactData) Global Native

Form Function GetPlacedObject(Explosion akExplosion) Global Native
Function SetPlacedObject(Explosion akExplosion, Form akForm) Global Native

float Function GetPlacedObjectAutoFadeDelay(Explosion akExplosion) Global Native
Function SetPlacedObjectAutoFadeDelay(Explosion akExplosion, float afValue) Global Native

Projectile Function GetSpawnedProjectile(Explosion akExplosion) Global Native
Function SetSpawnedProjectile(Explosion akExplosion, Projectile akProjectile) Global Native

Vector3 Function GetProjectileSpawnPosition(Explosion akExplosion) Global Native
Function SetProjectileSpawnPosition(Explosion akExplosion, Vector3 avValue) Global Native

float Function GetProjectileSpreadAngle(Explosion akExplosion) Global Native
Function SetProjectileSpreadAngle(Explosion akExplosion, float afValue) Global Native

int Function GetProjectileCount(Explosion akExplosion) Global Native
Function SetProjectileCount(Explosion akExplosion, int aiValue) Global Native

float Function GetForce(Explosion akExplosion) Global Native
Function SetForce(Explosion akExplosion, float afValue) Global Native

float Function GetDamage(Explosion akExplosion) Global Native
Function SetDamage(Explosion akExplosion, float afValue) Global Native

float Function GetInnerRadius(Explosion akExplosion) Global Native
Function SetInnerRadius(Explosion akExplosion, float afValue) Global Native

float Function GetOuterRadius(Explosion akExplosion) Global Native
Function SetOuterRadius(Explosion akExplosion, float afValue) Global Native

float Function GetImageSpaceRadius(Explosion akExplosion) Global Native
Function SetImageSpaceRadius(Explosion akExplosion, float afValue) Global Native

float Function GetVerticalOffsetMult(Explosion akExplosion) Global Native
Function SetVerticalOffsetMult(Explosion akExplosion, float afValue) Global Native

int Function GetSoundLevel(Explosion akExplosion) Global Native
Function SetSoundLevel(Explosion akExplosion, int aiValue) Global Native

int Function GetStaggerMagnitude(Explosion akExplosion) Global Native
Function SetStaggerMagnitude(Explosion akExplosion, int aiValue) Global Native


bool Function HasFlag(Explosion akExplosion, int aiFlag) Global Native
int Function GetFlags(Explosion akExplosion) Global Native
Function SetFlags(Explosion akExplosion, int aiFlags) Global Native
Function SetFlag(Explosion akExplosion, int aiFlag, bool abSet) Global Native

;/
	Explosion Flags:
/;

int Function GetFlag_AlwaysUseWorldOrientation() Global Native
int Function GetFlag_AlwaysKnockDown() Global Native
int Function GetFlag_KnockDownByFormula() Global Native
int Function GetFlag_IgnoreLosCheck() Global Native
int Function GetFlag_PushExplosionSourceRefOnly() Global Native
int Function GetFlag_IgnoreImageSpaceSwap() Global Native
int Function GetFlag_HasChain() Global Native
int Function GetFlag_HasNoControllerVibration() Global Native
int Function GetFlag_PlacedObjectPersist() Global Native
int Function GetFlag_SkipUnderwaterTest() Global Native
