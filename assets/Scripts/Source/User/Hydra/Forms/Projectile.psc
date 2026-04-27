Scriptname Hydra:Forms:Projectile Const Hidden Native

;/
	Provides functions for projectiles.

	Sound Levels:
	00	Loud
	01	Normal
	02	Silent
	03	Very Loud
	04	Quiet
/;

int Function GetType(Projectile akProjectile) Global Native
Function SetType(Projectile akProjectile, int aiValue) Global Native

int Function GetType_Missile() Global Native
int Function GetType_Lobber() Global Native
int Function GetType_Beam() Global Native
int Function GetType_Flame() Global Native
int Function GetType_Cone() Global Native
int Function GetType_Barrier() Global Native
int Function GetType_Arrow() Global Native

float Function GetSpeed(Projectile akProjectile) Global Native
Function SetSpeed(Projectile akProjectile, float afValue) Global Native

float Function GetGravity(Projectile akProjectile) Global Native
Function SetGravity(Projectile akProjectile, float afValue) Global Native

float Function GetRange(Projectile akProjectile) Global Native
Function SetRange(Projectile akProjectile, float afValue) Global Native

float Function GetImpactForce(Projectile akProjectile) Global Native
Function SetImpactForce(Projectile akProjectile, float afValue) Global Native

int Function GetTracerFrequency(Projectile akProjectile) Global Native
Function SetTracerFrequency(Projectile akProjectile, int aiValue) Global Native

float Function GetFadeDuration(Projectile akProjectile) Global Native
Function SetFadeDuration(Projectile akProjectile, float afValue) Global Native

float Function GetConeSpread(Projectile akProjectile) Global Native
Function SetConeSpread(Projectile akProjectile, float afValue) Global Native

float Function GetCollisionRadius(Projectile akProjectile) Global Native
Function SetCollisionRadius(Projectile akProjectile, float afValue) Global Native

float Function GetLifetime(Projectile akProjectile) Global Native
Function SetLifetime(Projectile akProjectile, float afValue) Global Native

float Function GetRelaunchInterval(Projectile akProjectile) Global Native
Function SetRelaunchInterval(Projectile akProjectile, float afValue) Global Native

Light Function GetMuzzleFlashLight(Projectile akProjectile) Global Native
Function SetMuzzleFlashLight(Projectile akProjectile, Light akValue) Global Native

float Function GetMuzzleFlashDuration(Projectile akProjectile) Global Native
Function SetMuzzleFlashDuration(Projectile akProjectile, float afValue) Global Native

Light Function GetLight(Projectile akProjectile) Global Native
Function SetLight(Projectile akProjectile, Light akValue) Global Native

Weapon Function GetSourceWeapon(Projectile akProjectile) Global Native
Function SetSourceWeapon(Projectile akProjectile, Weapon akValue) Global Native

TextureSet Function GetDecal(Projectile akProjectile) Global Native
Function SetDecal(Projectile akProjectile, TextureSet akValue) Global Native

Sound Function GetSound(Projectile akProjectile) Global Native
Function SetSound(Projectile akProjectile, Sound akValue) Global Native

Sound Function GetCountdownSound(Projectile akProjectile) Global Native
Function SetCountdownSound(Projectile akProjectile, Sound akValue) Global Native

Sound Function GetDisarmSound(Projectile akProjectile) Global Native
Function SetDisarmSound(Projectile akProjectile, Sound akValue) Global Native

int Function GetSoundLevel(Projectile akProjectile) Global Native
Function SetSoundLevel(Projectile akProjectile, int aiValue) Global Native

Explosion Function GetExplosion(Projectile akProjectile) Global Native
Function SetExplosion(Projectile akProjectile, Explosion akValue) Global Native

float Function GetExplosionTimer(Projectile akProjectile) Global Native
Function SetExplosionTimer(Projectile akProjectile, float afValue) Global Native

float Function GetExplosionProximity(Projectile akProjectile) Global Native
Function SetExplosionProximity(Projectile akProjectile, float afValue) Global Native

CollisionLayer Function GetCollisionLayer(Projectile akProjectile) Global Native
Function SetCollisionLayer(Projectile akProjectile, CollisionLayer akValue) Global Native

Projectile Function GetVatsProjectile(Projectile akProjectile) Global Native
Function SetVatsProjectile(Projectile akProjectile, Projectile akValue) Global Native


bool Function HasFlag(Projectile akProjectile, int aiFlag) Global Native
int Function GetFlags(Projectile akProjectile) Global Native
Function SetFlags(Projectile akProjectile, int aiFlags) Global Native
Function SetFlag(Projectile akProjectile, int aiFlag, bool abSet) Global Native

;/
	Projectile Flags:
/;

int Function GetFlag_HasHitScan() Global Native
int Function GetFlag_HasExplosion() Global Native
int Function GetFlag_HasAltExplosionTrigger() Global Native
int Function GetFlag_HasMuzzleFlash() Global Native
int Function GetFlag_CanBeDisarmed() Global Native
int Function GetFlag_CanBePickedUp() Global Native
int Function GetFlag_IsSupersonic() Global Native
int Function GetFlag_PinLimbs() Global Native
int Function GetFlag_PassSmallTransparent() Global Native
int Function GetFlag_DisableCombatAimCorrection() Global Native
int Function GetFlag_PenetrateGeometry() Global Native
int Function GetFlag_UpdateContinuously() Global Native
int Function GetFlag_SeekTarget() Global Native
