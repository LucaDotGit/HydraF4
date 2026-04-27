Scriptname Hydra:Forms:Container Const Hidden Native

;/
	Provides functions for containers.
/;

Sound Function GetOpenSound(Container akContainer) Global Native
Function SetOpenSound(Container akContainer, Sound akValue) Global Native

Sound Function GetCloseSound(Container akContainer) Global Native
Function SetCloseSound(Container akContainer, Sound akValue) Global Native

Sound Function GetTakeAllSound(Container akContainer) Global Native
Function SetTakeAllSound(Container akContainer, Sound akValue) Global Native

FormList Function GetFilterList(Container akContainer) Global Native
Function SetFilterList(Container akContainer, FormList akValue) Global Native

Hydra:Forms:Form:ContainerEntry[] Function GetItems(Container akContainer) Global
	return Hydra:Forms:Form.GetContainerItems(akContainer)
EndFunction
Function SetItems(Container akContainer, Hydra:Forms:Form:ContainerEntry[] akValues) Global
	Hydra:Forms:Form.SetContainerItems(akContainer, akValues)
EndFunction


bool Function HasFlag(Container akContainer, int aiFlag) Global Native
int Function GetFlags(Container akContainer) Global Native
Function SetFlags(Container akContainer, int aiFlags) Global Native
Function SetFlag(Container akContainer, int aiFlag, bool abSet) Global Native

;/
	Container Flags:
/;

int Function GetFlag_AllowSoundOnAnim() Global Native
int Function GetFlag_Respawn() Global Native
int Function GetFlag_ShowOwner() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_HasDistantLod() Global Native
int Function GetRecordFlag_HasRandomAnimStart() Global Native
int Function GetRecordFlag_IsObstacle() Global Native
int Function GetRecordFlag_HasNavMeshFilter() Global Native
int Function GetRecordFlag_HasNavMeshBoundingBox() Global Native
int Function GetRecordFlag_HasNavMeshGround() Global Native
