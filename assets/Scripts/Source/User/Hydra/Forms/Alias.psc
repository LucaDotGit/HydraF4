Scriptname Hydra:Forms:Alias Const Hidden Native

;/
	Provides functions for aliases.
/;

int Function GetId(Alias akAlias) Global Native
string Function GetName(Alias akAlias) Global Native
Quest Function GetOwningQuest(Alias akAlias) Global Native


bool Function HasFlag(Alias akAlias, int aiFlag) Global Native
int Function GetFlags(Alias akAlias) Global Native
Function SetFlags(Alias akAlias, int aiFlags) Global Native
Function SetFlag(Alias akAlias, int aiFlag, bool abValue) Global Native

;/
	Alias Flags:
/;

int Function GetFlag_Reserve() Global Native
int Function GetFlag_IsOptional() Global Native
int Function GetFlag_IsQuestObject() Global Native
int Function GetFlag_AllowReuse() Global Native
int Function GetFlag_AllowDead() Global Native
int Function GetFlag_AllowLoadedOnly() Global Native
int Function GetFlag_IsEssential() Global Native
int Function GetFlag_AllowDisabled() Global Native
int Function GetFlag_StoreName() Global Native
int Function GetFlag_AllowReserved() Global Native
int Function GetFlag_IsProtected() Global Native
int Function GetFlag_IsForcedAlias() Global Native
int Function GetFlag_AllowDestroyed() Global Native
int Function GetFlag_FindRefClosest() Global Native
int Function GetFlag_UseName() Global Native
int Function GetFlag_IsInitiallyDisabled() Global Native
int Function GetFlag_AllowCleared() Global Native
int Function GetFlag_ClearNameOnRemoval() Global Native
int Function GetFlag_AllowActorsOnly() Global Native
int Function GetFlag_IsTransient() Global Native
int Function GetFlag_IsLinkExternal() Global Native
int Function GetFlag_DisallowPickpocketing() Global Native
int Function GetFlag_IsDataAlias() Global Native
int Function GetFlag_IsCompanion() Global Native
int Function GetFlag_IsSceneOptional() Global Native
