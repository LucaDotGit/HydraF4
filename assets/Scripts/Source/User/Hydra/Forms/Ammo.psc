Scriptname Hydra:Forms:Ammo Const Hidden Native

;/
	Provides functions for ammo.
/;

Projectile Function GetProjectile(Ammo akAmmo) Global Native
Function SetProjectile(Ammo akAmmo, Projectile akValue) Global Native

int Function GetHealth(Ammo akAmmo) Global Native
Function SetHealth(Ammo akAmmo, int aiValue) Global Native

float Function GetDamage(Ammo akAmmo) Global Native
Function SetDamage(Ammo akAmmo, float afValue) Global Native

string Function GetShortName(Ammo akAmmo) Global Native
Function SetShortName(Ammo akAmmo, string acsValue) Global Native


bool Function HasFlag(Ammo akAmmo, int aiFlag) Global Native
int Function GetFlags(Ammo akAmmo) Global Native
Function SetFlags(Ammo akAmmo, int aiFlags) Global Native
Function SetFlag(Ammo akAmmo, int aiFlag, bool abSet) Global Native

;/
	Ammo Flags:
/;

int Function GetFlag_IgnoreNormalWeaponResist() Global Native
int Function GetFlag_IsNonPlayable() Global Native
int Function GetFlag_HasCountBased3D() Global Native
