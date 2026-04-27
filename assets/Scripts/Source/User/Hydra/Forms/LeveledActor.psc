Scriptname Hydra:Forms:LeveledActor Const Hidden Native

;/
	Provides functions for leveled actors.

	Notes:
	- Leveled actors only support up to 255 entries.
/;

Import Hydra:Forms:LeveledList

int Function GetMaxUseAllCount(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetMaxUseAllCount(akLeveledActor)
EndFunction
Function SetMaxUseAllCount(LeveledActor akLeveledActor, int aiValue) Global
	Hydra:Forms:LeveledList.SetMaxUseAllCount(akLeveledActor, aiValue)
EndFunction

int Function GetChanceNone(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetChanceNone(akLeveledActor)
EndFunction
Function SetChanceNone(LeveledActor akLeveledActor, int aiValue) Global
	Hydra:Forms:LeveledList.SetChanceNone(akLeveledActor, aiValue)
EndFunction

GlobalVariable Function GetChanceNoneGlobal(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetChanceNoneGlobal(akLeveledActor)
EndFunction
Function SetChanceNoneGlobal(LeveledActor akLeveledActor, GlobalVariable akValue) Global
	Hydra:Forms:LeveledList.SetChanceNoneGlobal(akLeveledActor, akValue)
EndFunction

int Function GetEntryCount(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetEntryCount(akLeveledActor)
EndFunction
int Function GetEditorEntryCount(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetEditorEntryCount(akLeveledActor)
EndFunction
int Function GetScriptEntryCount(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetScriptEntryCount(akLeveledActor)
EndFunction

LeveledEntry[] Function GetAllEntries(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetAllEntries(akLeveledActor)
EndFunction

LeveledEntry[] Function GetEditorEntries(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetEditorEntries(akLeveledActor)
EndFunction
Function SetEditorEntries(LeveledActor akLeveledActor, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetEditorEntries(akLeveledActor, akValues)
EndFunction

LeveledEntry[] Function GetScriptEntries(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetScriptEntries(akLeveledActor)
EndFunction
Function SetScriptEntries(LeveledActor akLeveledActor, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetScriptEntries(akLeveledActor, akValues)
EndFunction

LeveledEntry Function GetNthEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthEntry(akLeveledActor, aiIndex)
EndFunction
bool Function SetNthEntry(LeveledActor akLeveledActor, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthEntry(akLeveledActor, aiIndex, akEntry)
EndFunction
bool Function RemoveNthEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthEntry(akLeveledActor, aiIndex)
EndFunction

LeveledEntry Function GetNthEditorEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthEditorEntry(akLeveledActor, aiIndex)
EndFunction
bool Function SetNthEditorEntry(LeveledActor akLeveledActor, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthEditorEntry(akLeveledActor, aiIndex, akEntry)
EndFunction
bool Function RemoveNthEditorEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthEditorEntry(akLeveledActor, aiIndex)
EndFunction

LeveledEntry Function GetNthScriptEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthScriptEntry(akLeveledActor, aiIndex)
EndFunction
bool Function SetNthScriptEntry(LeveledActor akLeveledActor, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthScriptEntry(akLeveledActor, aiIndex, akEntry)
EndFunction
bool Function RemoveNthScriptEntry(LeveledActor akLeveledActor, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthScriptEntry(akLeveledActor, aiIndex)
EndFunction

bool Function AddEditorEntry(LeveledActor akLeveledActor, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.AddEditorEntry(akLeveledActor, akEntry)
EndFunction
bool Function AddScriptEntry(LeveledActor akLeveledActor, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.AddScriptEntry(akLeveledActor, akEntry)
EndFunction

Function AddEditorEntries(LeveledActor akLeveledActor, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.AddEditorEntries(akLeveledActor, akValues)
EndFunction
Function AddScriptEntries(LeveledActor akLeveledActor, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.AddScriptEntries(akLeveledActor, akValues)
EndFunction

Function ClearEntries(LeveledActor akLeveledActor) Global
	Hydra:Forms:LeveledList.ClearEntries(akLeveledActor)
EndFunction
Function ClearEditorEntries(LeveledActor akLeveledActor) Global
	Hydra:Forms:LeveledList.ClearEditorEntries(akLeveledActor)
EndFunction
Function ClearScriptEntries(LeveledActor akLeveledActor) Global
	Hydra:Forms:LeveledList.ClearScriptEntries(akLeveledActor)
EndFunction

KeywordEntry[] Function GetKeywordChances(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetKeywordChances(akLeveledActor)
EndFunction
Function SetKeywordChances(LeveledActor akLeveledActor, KeywordEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetKeywordChances(akLeveledActor, akValues)
EndFunction


bool Function HasFlag(LeveledActor akLeveledActor, int aiFlag) Global
	return Hydra:Forms:LeveledList.HasFlag(akLeveledActor, aiFlag)
EndFunction
int Function GetFlags(LeveledActor akLeveledActor) Global
	return Hydra:Forms:LeveledList.GetFlags(akLeveledActor)
EndFunction
Function SetFlags(LeveledActor akLeveledActor, int aiFlags) Global
	Hydra:Forms:LeveledList.SetFlags(akLeveledActor, aiFlags)
EndFunction
Function SetFlag(LeveledActor akLeveledActor, int aiFlag, bool abSet) Global
	Hydra:Forms:LeveledList.SetFlag(akLeveledActor, aiFlag, abSet)
EndFunction

;/
	Leveled Actor Flags:
/;

int Function GetFlag_CalculateAllLessEqualsPlayerLevel() Global
	return Hydra:Forms:LeveledList.GetFlag_CalculateAllLessEqualsPlayerLevel()
EndFunction
int Function GetFlag_CalculateEachItemInCount() Global
	return Hydra:Forms:LeveledList.GetFlag_CalculateEachItemInCount()
EndFunction
int Function GetFlag_UseAll() Global
	return Hydra:Forms:LeveledList.GetFlag_UseAll()
EndFunction
