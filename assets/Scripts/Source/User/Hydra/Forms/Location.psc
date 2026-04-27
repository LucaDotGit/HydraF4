Scriptname Hydra:Forms:Location Const Hidden Native

;/
	Provides functions for locations.

	Notes:
	- Changing the location's clear state here only changes the internal flag
	  and does not update the associated misc stat or the UI.
/;

bool Function IsChildOf(Location akChildLocation, Location akParentLocation) Global Native
bool Function IsParentOf(Location akParentLocation, Location akChildLocation) Global Native

Location Function GetParent(Location akLocation) Global Native
Function SetParent(Location akLocation, Location akValue) Global Native

MusicType Function GetMusicType(Location akLocation) Global Native
Function SetMusicType(Location akLocation, MusicType akValue) Global Native

EncounterZone Function GetEncounterZone(Location akLocation) Global Native
Function SetEncounterZone(Location akLocation, EncounterZone akValue) Global Native

ObjectReference Function GetMapMarkerRef(Location akLocation) Global Native
Function SetMapMarkerRef(Location akLocation, ObjectReference akValue) Global Native

float Function GetWorldRadius(Location akLocation) Global Native
Function SetWorldRadius(Location akLocation, float afValue) Global Native

float Function GetActorFadeMult(Location akLocation) Global Native
Function SetActorFadeMult(Location akLocation, float afValue) Global Native

bool Function GetIsCleared(Location akLocation) Global Native
Function SetIsCleared(Location akLocation, bool abValue) Global Native

bool Function GetHasEverBeenCleared(Location akLocation) Global Native
Function SetHasEverBeenCleared(Location akLocation, bool abValue) Global Native

Location[] Function GetChildLocations(Location akParentLocation) Global Native

int Function GetLocationLinkCount(Location akLocation) Global Native

Location Function GetNthLocationLinkLocation(Location akLocation, int aiIndex) Global Native
Keyword Function GetNthLocationLinkKeyword(Location akLocation, int aiIndex) Global Native

int Function GetKeywordDataCount(Location akLocation) Global Native

Keyword Function GetNthKeywordDataKeyword(Location akLocation, int aiIndex) Global Native

float Function GetNthKeywordDataValue(Location akLocation, int aiIndex) Global Native
bool Function SetNthKeywordDataValue(Location akLocation, int aiIndex, float afValue) Global Native

int Function GetKeywordDataIndexByKeyword(Location akLocation, Keyword akKeyword) Global Native
