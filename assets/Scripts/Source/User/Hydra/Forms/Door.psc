Scriptname Hydra:Forms:Door Const Hidden Native

;/
	Provides functions for doors.
/;

Sound Function GetOpenSound(Door akDoor) Global Native
Function SetOpenSound(Door akDoor, Sound akValue) Global Native

Sound Function GetCloseSound(Door akDoor) Global Native
Function SetCloseSound(Door akDoor, Sound akValue) Global Native

Sound Function GetLoopSound(Door akDoor) Global Native
Function SetLoopSound(Door akDoor, Sound akValue) Global Native

string Function GetAltOpenText(Door akDoor) Global Native
Function SetAltOpenText(Door akDoor, string acsValue) Global Native

string Function GetAltCloseText(Door akDoor) Global Native
Function SetAltCloseText(Door akDoor, string acsValue) Global Native

bool Function ContainsRandomTeleporter(Door akDoor, Form akTeleporter) Global Native
Form[] Function GetRandomTeleporters(Door akDoor) Global Native


bool Function HasFlag(Door akDoor, int aiFlag) Global Native
int Function GetFlags(Door akDoor) Global Native
Function SetFlags(Door akDoor, int aiFlags) Global Native
Function SetFlag(Door akDoor, int aiFlag, bool abSet) Global Native

;/
	Door Flags:
/;

int Function GetFlag_IsAutomatic() Global Native
int Function GetFlag_IsHidden() Global Native
int Function GetFlag_IsUsedMinimally() Global Native
int Function GetFlag_IsSlidable() Global Native
int Function GetFlag_DisallowOpeningInCombatSearch() Global Native
int Function GetFlag_HasNoDestinationText() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_HasDistantLod() Global Native
int Function GetRecordFlag_HasRandomAnimStart() Global Native
int Function GetRecordFlag_IsMarker() Global Native
