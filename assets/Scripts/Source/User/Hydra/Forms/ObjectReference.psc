Scriptname Hydra:Forms:ObjectReference Const Hidden Native

;/
	Provides functions for object references.

	Notes:
	- Angles and rotations are measured in degrees.
	- Positions are measured in game units, see https://ck.uesp.net/wiki/Unit.
	- The position functions don't allow moving the player beyond one cell (4096 game units).
	- For a list of all map marker types, see:
	  https://github.com/shad0wshayd3-FO4/CommonLibF4/blob/01d7eeef39fd5d812595062a53af7a40e9f3e401/CommonLibF4/include/RE/Bethesda/BSExtraData.h#L383
/;

Import Hydra:Vectors3

Form Function GetBaseObject(ObjectReference akRef) Global Native
Function SetBaseObject(ObjectReference akRef, Form akValue) Global Native

int Function GetRecordFlag_IsCollisionDisabled() Global Native
int Function GetRecordFlag_IsInitiallyDead() Global Native
int Function GetRecordFlag_IsPersistent() Global Native
int Function GetRecordFlag_IsInitiallyDisabled() Global Native
int Function GetRecordFlag_IsInitiallyUnconscious() Global Native
int Function GetRecordFlag_DoesRespawn() Global Native
int Function GetRecordFlag_IsHarvested() Global Native
int Function GetRecordFlag_IsFullLod() Global Native

float Function GetPositionX(ObjectReference akRef) Global Native
bool Function SetPositionX(ObjectReference akRef, float afValue) Global Native
bool Function ModPositionX(ObjectReference akRef, float afAmount) Global Native

float Function GetPositionY(ObjectReference akRef) Global Native
bool Function SetPositionY(ObjectReference akRef, float afValue) Global Native
bool Function ModPositionY(ObjectReference akRef, float afAmount) Global Native

float Function GetPositionZ(ObjectReference akRef) Global Native
bool Function SetPositionZ(ObjectReference akRef, float afValue) Global Native
bool Function ModPositionZ(ObjectReference akRef, float afAmount) Global Native

float Function GetRotationX(ObjectReference akRef) Global Native
bool Function SetRotationX(ObjectReference akRef, float afValue) Global Native
bool Function ModRotationX(ObjectReference akRef, float afAmount) Global Native

float Function GetRotationY(ObjectReference akRef) Global Native
bool Function SetRotationY(ObjectReference akRef, float afValue) Global Native
bool Function ModRotationY(ObjectReference akRef, float afAmount) Global Native

float Function GetRotationZ(ObjectReference akRef) Global Native
bool Function SetRotationZ(ObjectReference akRef, float afValue) Global Native
bool Function ModRotationZ(ObjectReference akRef, float afAmount) Global Native

Vector3 Function GetPosition(ObjectReference akRef) Global Native
bool Function SetPosition(ObjectReference akRef, Vector3 akPosition) Global Native

Vector3 Function GetRotation(ObjectReference akRef) Global Native
bool Function SetRotation(ObjectReference akRef, Vector3 akRotation) Global Native

float Function GetHeadingRotation(ObjectReference akRef, ObjectReference akTargetRef) Global Native
bool Function SetHeadingRotation(ObjectReference akRef, ObjectReference akTargetRef, float afOffset = 0.0) Global Native

bool Function MoveAround(ObjectReference akRef, ObjectReference akTargetRef, float afDistance, \
	float afAngle = 0.0, float afHeightOffset = 0.0, float afRotationOffset = 0.0, bool abFaceTarget = false, bool abRetainRotation = false) Global Native

Cell Function GetCurrentCell(ObjectReference akRef) Global Native
Location Function GetCurrentLocation(ObjectReference akRef) Global Native

Location Function GetLocationByMapMarker(ObjectReference akMarkerRef) Global Native

Struct MapMarkerData
	int iType = 0
	string sName
EndStruct

MapMarkerData Function GetMapMarkerData(ObjectReference akMarkerRef) Global Native
bool Function SetMapMarkerData(ObjectReference akMarkerRef, MapMarkerData akData) Global Native

Struct DoorDestinationData
	ObjectReference kLinkedDoor
	Vector3 kPosition
	Vector3 kRotation
EndStruct

DoorDestinationData Function GetDoorDestination(ObjectReference akDoorRef) Global Native
bool Function SetDoorDestination(ObjectReference akDoorRef, DoorDestinationData akData) Global Native

Struct AliasInstanceData
	Quest kOwnerQuest
	ReferenceAlias kLinkedAlias
	Package[] kPackages
EndStruct

AliasInstanceData[] Function GetAliasInstances(ObjectReference akRef) Global Native
AliasInstanceData[] Function GetAliasInstancesByQuest(ObjectReference akRef, Quest akQuest) Global Native

string Function GetDisplayName(ObjectReference akRef) Global Native
Function SetDisplayName(ObjectReference akRef, string acsValue) Global Native

ObjectReference Function GetAshPileSourceRef(ObjectReference akAshPileRef) Global Native
Function SetAshPileSourceRef(ObjectReference akAshPileRef, ObjectReference akSourceRef) Global Native

ObjectReference Function GetDroppedItemSourceRef(ObjectReference akItemRef) Global Native
Function SetDroppedItemSourceREf(ObjectReference akItemRef, ObjectReference akSourceRef) Global Native

ObjectReference[] Function GetDroppedItemRefs(ObjectReference akItemRef) Global Native
Function SetDroppedItemRefs(ObjectReference akItemRef, ObjectReference[] akSourceRefs) Global Native

int Function GetItemStackCount(ObjectReference akRef) Global Native
Function SetItemStackCount(ObjectReference akRef, int aiValue) Global Native

int Function GetLockTryCount(ObjectReference akRef) Global Native
Function SetLockTryCount(ObjectReference akRef, int aiValue) Global Native

Key Function GetLockKey(ObjectReference akRef) Global Native
Function SetLockKey(ObjectReference akRef, Key akValue) Global Native
