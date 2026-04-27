Scriptname Hydra:Forms:EncounterZone Const Hidden Native

;/
	Provides functions for encounter zones.
/;

Form Function GetOwner(EncounterZone akEncounterZone) Global Native
Function SetOwner(EncounterZone akEncounterZone, Form akValue) Global Native

int Function GetOwnerRank(EncounterZone akEncounterZone) Global Native
Function SetOwnerRank(EncounterZone akEncounterZone, int aiValue) Global Native

Location Function GetLocation(EncounterZone akEncounterZone) Global Native
Function SetLocation(EncounterZone akEncounterZone, Location akValue) Global Native

int Function GetMinLevel(EncounterZone akEncounterZone) Global Native
Function SetMinLevel(EncounterZone akEncounterZone, int aiValue) Global Native

int Function GetMaxLevel(EncounterZone akEncounterZone) Global Native
Function SetMaxLevel(EncounterZone akEncounterZone, int aiValue) Global Native

int Function GetLevel(EncounterZone akEncounterZone) Global Native
Function SetLevel(EncounterZone akEncounterZone, int aiValue) Global Native

int Function GetDetachTime(EncounterZone akEncounterZone) Global Native
Function SetDetachTime(EncounterZone akEncounterZone, int aiValue) Global Native

int Function GetAttachTime(EncounterZone akEncounterZone) Global Native
Function SetAttachTime(EncounterZone akEncounterZone, int aiValue) Global Native

int Function GetResetTime(EncounterZone akEncounterZone) Global Native
Function SetResetTime(EncounterZone akEncounterZone, int aiValue) Global Native


bool Function HasFlag(EncounterZone akEncounterZone, int aiFlag) Global Native
int Function GetFlags(EncounterZone akEncounterZone) Global Native
Function SetFlags(EncounterZone akEncounterZone, int aiFlags) Global Native
Function SetFlag(EncounterZone akEncounterZone, int aiFlag, bool abSet) Global Native

;/
	Encounter Zone Flags:
/;

int Function GetFlag_NeverReset() Global Native
int Function GetFlag_MatchPlayerBelowMin() Global Native
int Function GetFlag_DisableCombatBoundary() Global Native
int Function GetFlag_IsWorkshopZone() Global Native
