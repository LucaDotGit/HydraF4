Scriptname Hydra:Forms:Light Const Hidden Native

;/
	Provides functions for lights.
/;

Import Hydra:Colors

Sound Function GetSound(Light akLight) Global Native
Function SetSound(Light akLight, Sound akValue) Global Native

float Function GetFov(Light akLight) Global Native
Function SetFov(Light akLight, float afValue) Global Native

float Function GetFalloffExponent(Light akLight) Global Native
Function SetFalloffExponent(Light akLight, float afValue) Global Native

int Function GetRadius(Light akLight) Global Native
Function SetRadius(Light akLight, int aiValue) Global Native

float Function GetNearDistance(Light akLight) Global Native
Function SetNearDistance(Light akLight, float afValue) Global Native

Color Function GetColor(Light akLight) Global Native
Function SetColor(Light akLight, Color akValue) Global Native

float Function GetColorFade(Light akLight) Global Native
Function SetColorFade(Light akLight, float afValue) Global Native

float Function GetFlickerPeriod(Light akLight) Global Native
Function SetFlickerPeriod(Light akLight, float afValue) Global Native

float Function GetFlickerIntensityAmplitude(Light akLight) Global Native
Function SetFlickerIntensityAmplitude(Light akLight, float afValue) Global Native

float Function GetFlickerMovementAmplitude(Light akLight) Global Native
Function SetFlickerMovementAmplitude(Light akLight, float afValue) Global Native

GodRays Function GetGodRays(Light akLight) Global Native
Function SetGodRays(Light akLight, GodRays akValue) Global Native

float Function GetGodRaysNearDistance(Light akLight) Global Native
Function SetGodRaysNearDistance(Light akLight, float afValue) Global Native

LensFlare Function GetLensFlare(Light akLight) Global Native
Function SetLensFlare(Light akLight, LensFlare akValue) Global Native

float Function GetAttenuationConstant(Light akLight) Global Native
Function SetAttenuationConstant(Light akLight, float afValue) Global Native

float Function GetAttenuationScalar(Light akLight) Global Native
Function SetAttenuationScalar(Light akLight, float afValue) Global Native

float Function GetAttenuationExponent(Light akLight) Global Native
Function SetAttenuationExponent(Light akLight, float afValue) Global Native


bool Function HasFlag(Light akLight, int aiFlag) Global Native
int Function GetFlags(Light akLight) Global Native
Function SetFlags(Light akLight, int aiFlags) Global Native
Function SetFlag(Light akLight, int aiFlag, bool abSet) Global Native

;/
	Light Flags:
/;

int Function GetFlag_IsDynamic() Global Native
int Function GetFlag_CanBeCarried() Global Native
int Function GetFlag_IsNegative() Global Native
int Function GetFlag_CanFlicker() Global Native
int Function GetFlag_IsDeepCopy() Global Native
int Function GetFlag_IsOffByDefault() Global Native
int Function GetFlag_FlickerSlowly() Global Native
int Function GetFlag_CanPulse() Global Native
int Function GetFlag_PulseSlowly() Global Native
int Function GetFlag_IsSpotlight() Global Native
int Function GetFlag_HasSpotlightShadow() Global Native
int Function GetFlag_HasHemiShadow() Global Native
int Function GetFlag_HasOmniShadow() Global Native
int Function GetFlag_IsPortalStrict() Global Native
int Function GetFlag_IsNonShadowSpotlight() Global Native
int Function GetFlag_IsNonSpecular() Global Native
int Function GetFlag_IsAttenuationOnly() Global Native
int Function GetFlag_IsNonShadowBox() Global Native
int Function GetFlag_IgnoreRoughness() Global Native
int Function GetFlag_HasNoRimLighting() Global Native
int Function GetFlag_IsAmbientOnly() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_HasDistantLod() Global Native
int Function GetRecordFlag_HasRandomAnimStart() Global Native
int Function GetRecordFlag_IsObstacle() Global Native
int Function GetRecordFlag_IsPortalStrict() Global Native
