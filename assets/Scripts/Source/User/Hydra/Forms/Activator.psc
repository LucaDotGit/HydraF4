Scriptname Hydra:Forms:Activator Const Hidden Native

;/
	Provides functions for activators.
/;

Sound Function GetLoopSound(Activator akActivator) Global Native
Function SetLoopSound(Activator akActivator, Sound akValue) Global Native

Sound Function GetActivateSound(Activator akActivator) Global Native
Function SetActivateSound(Activator akActivator, Sound akValue) Global Native

WaterType Function GetWaterType(Activator akActivator) Global Native
Function SetWaterType(Activator akActivator, WaterType akValue) Global Native


bool Function HasFlag(Activator akActivator, int aiFlag) Global Native
int Function GetFlags(Activator akActivator) Global Native
Function SetFlags(Activator akActivator, int aiFlags) Global Native
Function SetFlag(Activator akActivator, int aiFlag, bool abSet) Global Native

;/
	Activator Flags:
/;

int Function GetFlag_HasNoDisplacement() Global Native
int Function GetFlag_IsIgnoredBySandbox() Global Native
int Function GetFlag_IsProceduralWater() Global Native
int Function GetFlag_IsLodWater() Global Native
int Function GetFlag_IsRadio() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_NeverFade() Global Native
int Function GetRecordFlag_IsNonOccluder() Global Native
int Function GetRecordFlag_HasTreeLod() Global Native
int Function GetRecordFlag_IsHeadingMarker() Global Native
int Function GetRecordFlag_MustUpdateAnims() Global Native
int Function GetRecordFlag_IsHiddenFromLocalMap() Global Native
int Function GetRecordFlag_IsHeadtrackMarker() Global Native
int Function GetRecordFlag_IsUsedAsPlatform() Global Native
int Function GetRecordFlag_IsForPackInUseOnly() Global Native
int Function GetRecordFlag_HasDistantLod() Global Native
int Function GetRecordFlag_HasRandomAnimStart() Global Native
int Function GetRecordFlag_IsDangerous() Global Native
int Function GetRecordFlag_IgnoreObjectInteraction() Global Native
int Function GetRecordFlag_IsMarker() Global Native
int Function GetRecordFlag_IsObstacle() Global Native
int Function GetRecordFlag_HasNavMeshFilter() Global Native
int Function GetRecordFlag_HasNavMeshBoundingBox() Global Native
int Function GetRecordFlag_IsUsableByChildren() Global Native
int Function GetRecordFlag_HasNavMeshGround() Global Native
