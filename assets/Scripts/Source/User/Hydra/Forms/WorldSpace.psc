Scriptname Hydra:Forms:WorldSpace Const Hidden Native

;/
	Provides functions for world spaces.

	Notes:
	- Altering some of the below data may require the worldspace to be reloaded to take effect.
/;

Location Function GetLocation(WorldSpace akWorldSpace) Global Native
Function SetLocation(WorldSpace akWorldSpace, Location akValue) Global Native

EncounterZone Function GetEncounterZone(WorldSpace akWorldSpace) Global Native
Function SetEncounterZone(WorldSpace akWorldSpace, EncounterZone akValue) Global Native

LightingTemplate Function GetInteriorLighting(WorldSpace akWorldSpace) Global Native
Function SetInteriorLighting(WorldSpace akWorldSpace, LightingTemplate akValue) Global Native

Climate Function GetClimate(WorldSpace akWorldSpace) Global Native
Function SetClimate(WorldSpace akWorldSpace, Climate akValue) Global Native

MusicType Function GetMusicType(WorldSpace akWorldSpace) Global Native
Function SetMusicType(WorldSpace akWorldSpace, MusicType akValue) Global Native

WaterType Function GetWaterType(WorldSpace akWorldSpace) Global Native
Function SetWaterType(WorldSpace akWorldSpace, WaterType akValue) Global Native

WaterType Function GetLodWaterType(WorldSpace akWorldSpace) Global Native
Function SetLodWaterType(WorldSpace akWorldSpace, WaterType akValue) Global Native

float Function GetLodWaterHeight(WorldSpace akWorldSpace) Global Native
Function SetLodWaterHeight(WorldSpace akWorldSpace, float afValue) Global Native

Cell Function GetSkyCell(WorldSpace akWorldSpace) Global Native
Function SetSkyCell(WorldSpace akWorldSpace, Cell akValue) Global Native

bool Function ContainsLoadedCell(WorldSpace akWorldSpace, Cell akCell) Global Native
Cell[] Function GetLoadedCells(WorldSpace akWorldSpace) Global Native

bool Function ContainsLocation(WorldSpace akWorldSpace, Location akLocation) Global Native
Location[] Function GetLocations(WorldSpace akWorldSpace) Global Native


bool Function HasFlag(WorldSpace akWorldSpace, int aiFlag) Global Native
int Function GetFlags(WorldSpace akWorldSpace) Global Native
Function SetFlags(WorldSpace akWorldSpace, int aiFlags) Global Native
Function SetFlag(WorldSpace akWorldSpace, int aiFlag, bool abSet) Global Native

;/
	World Space Flags:
/;

int Function GetFlag_IsSmallWorld() Global Native
int Function GetFlag_DisallowFastTravel() Global Native
int Function GetFlag_HasNoLodWater() Global Native
int Function GetFlag_HasNoLandscape() Global Native
int Function GetFlag_HasNoSky() Global Native
int Function GetFlag_HasFixedDimensions() Global Native
int Function GetFlag_HasNoGrass() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_DisallowWaiting() Global Native
