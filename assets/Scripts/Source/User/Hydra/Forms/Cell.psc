Scriptname Hydra:Forms:Cell Const Hidden Native

;/
	Provides functions for cells.

	Notes:
	- Altering some of the below data may require the cell to be reloaded to take effect.
	- Distances are measured in game units, see https://ck.uesp.net/wiki/Unit.
	- The `Var[]` filters support the following types:
	  - form types (string)
	  - plugin names (string)
	  - references
	  - base objects
	  - keywords
	  - factions
	  - races
	  - form lists (recursive)
	- For a list of all form types,
	  see: https://github.com/libxse/commonlibf4/blob/main/include/RE/E/ENUM_FORM_ID.h.
/;

Import Hydra:Vectors3

Form Function GetOwner(Cell akCell) Global Native
Function SetOwner(Cell akCell, Form akValue) Global Native

Location Function GetLocation(Cell akCell) Global Native
Function SetLocation(Cell akCell, Location akValue) Global Native

EncounterZone Function GetEncounterZone(Cell akCell) Global Native
Function SetEncounterZone(Cell akCell, EncounterZone akValue) Global Native

AcousticSpace Function GetAcousticSpace(Cell akCell) Global Native
Function SetAcousticSpace(Cell akCell, AcousticSpace akValue) Global Native

LightingTemplate Function GetLightingTemplate(Cell akCell) Global Native
Function SetLightingTemplate(Cell akCell, LightingTemplate akValue) Global Native

GodRays Function GetGodRays(Cell akCell) Global Native
Function SetGodRays(Cell akCell, GodRays akValue) Global Native

ImageSpace Function GetImageSpace(Cell akCell) Global Native
Function SetImageSpace(Cell akCell, ImageSpace akValue) Global Native

MusicType Function GetMusicType(Cell akCell) Global Native
Function SetMusicType(Cell akCell, MusicType akValue) Global Native

WaterType Function GetWaterType(Cell akCell) Global Native
Function SetWaterType(Cell akCell, WaterType akValue) Global Native

Region Function GetSkyRegion(Cell akCell) Global Native
Function SetSkyRegion(Cell akCell, Region akValue) Global Native

Region[] Function GetRegions(Cell akCell) Global Native
Function SetRegions(Cell akCell, Region[] akValues) Global Native

ObjectReference[] Function FindAllRefs(Cell akOriginCell, \
	Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native
ObjectReference[] Function FindAllRefsInRange(Cell akOriginCell, Vector3 akCenter, float afRadius, \
	Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native
ObjectReference[] Function FindAllRefsInRangeFromRef(ObjectReference akOriginRef, float afRadius, \
	Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native


bool Function HasFlag(Cell akCell, int aiFlag) Global Native
int Function GetFlags(Cell akCell) Global Native
Function SetFlags(Cell akCell, int aiFlags) Global Native
Function SetFlag(Cell akCell, int aiFlag, bool abSet) Global Native

;/
	Cell Flags:
/;

int Function GetFlag_IsInterior() Global Native
int Function GetFlag_HasWater() Global Native
int Function GetFlag_AllowFastTravel() Global Native
int Function GetFlag_HasNoLodWater() Global Native
int Function GetFlag_HasTempData() Global Native
int Function GetFlag_IsPublicArea() Global Native
int Function GetFlag_IsHandChanged() Global Native
int Function GetFlag_ShowSky() Global Native
int Function GetFlag_UseSkyLighting() Global Native
int Function GetFlag_HasExitWarning() Global Native
int Function GetFlag_IsHiddenFromInteriorList() Global Native
int Function GetFlag_UseSunlightShadows() Global Native
int Function GetFlag_HasDistantLodOnly() Global Native
int Function GetFlag_DisallowCompanionFastTravel() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_HasNoPreVis() Global Native
int Function GetRecordFlag_IsPersistent() Global Native
int Function GetRecordFlag_IsOffLimits() Global Native
int Function GetRecordFlag_IsCompressed() Global Native
int Function GetRecordFlag_DisallowWaiting() Global Native
