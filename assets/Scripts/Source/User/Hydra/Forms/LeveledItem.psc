Scriptname Hydra:Forms:LeveledItem Const Hidden Native

;/
	Provides functions for leveled items.

	Notes:
	- Leveled items only support up to 255 entries.
/;

Import Hydra:Forms:LeveledList

int Function GetMaxUseAllCount(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetMaxUseAllCount(akLeveledItem)
EndFunction
Function SetMaxUseAllCount(LeveledItem akLeveledItem, int aiValue) Global
	Hydra:Forms:LeveledList.SetMaxUseAllCount(akLeveledItem, aiValue)
EndFunction

int Function GetChanceNone(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetChanceNone(akLeveledItem)
EndFunction
Function SetChanceNone(LeveledItem akLeveledItem, int aiValue) Global
	Hydra:Forms:LeveledList.SetChanceNone(akLeveledItem, aiValue)
EndFunction

GlobalVariable Function GetChanceNoneGlobal(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetChanceNoneGlobal(akLeveledItem)
EndFunction
Function SetChanceNoneGlobal(LeveledItem akLeveledItem, GlobalVariable akValue) Global
	Hydra:Forms:LeveledList.SetChanceNoneGlobal(akLeveledItem, akValue)
EndFunction

GlobalVariable Function GetLegendaryChanceGlobal(LeveledItem akLeveledItem) Global Native
Function SetLegendaryChanceGlobal(LeveledItem akLeveledItem, GlobalVariable akValue) Global Native

string Function GetOverrideName(LeveledItem akLeveledItem) Global Native
Function SetOverrideName(LeveledItem akLeveledItem, string acsValue) Global Native

int Function GetEntryCount(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetEntryCount(akLeveledItem)
EndFunction
int Function GetEditorEntryCount(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetEditorEntryCount(akLeveledItem)
EndFunction
int Function GetScriptEntryCount(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetScriptEntryCount(akLeveledItem)
EndFunction

LeveledEntry[] Function GetAllEntries(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetAllEntries(akLeveledItem)
EndFunction

LeveledEntry[] Function GetEditorEntries(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetEditorEntries(akLeveledItem)
EndFunction
Function SetEditorEntries(LeveledItem akLeveledItem, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetEditorEntries(akLeveledItem, akValues)
EndFunction

LeveledEntry[] Function GetScriptEntries(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetScriptEntries(akLeveledItem)
EndFunction
Function SetScriptEntries(LeveledItem akLeveledItem, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetScriptEntries(akLeveledItem, akValues)
EndFunction

LeveledEntry Function GetNthEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthEntry(akLeveledItem, aiIndex)
EndFunction
bool Function SetNthEntry(LeveledItem akLeveledItem, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthEntry(akLeveledItem, aiIndex, akEntry)
EndFunction
bool Function RemoveNthEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthEntry(akLeveledItem, aiIndex)
EndFunction

LeveledEntry Function GetNthEditorEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthEditorEntry(akLeveledItem, aiIndex)
EndFunction
bool Function SetNthEditorEntry(LeveledItem akLeveledItem, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthEditorEntry(akLeveledItem, aiIndex, akEntry)
EndFunction
bool Function RemoveNthEditorEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthEditorEntry(akLeveledItem, aiIndex)
EndFunction

LeveledEntry Function GetNthScriptEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.GetNthScriptEntry(akLeveledItem, aiIndex)
EndFunction
bool Function SetNthScriptEntry(LeveledItem akLeveledItem, int aiIndex, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.SetNthScriptEntry(akLeveledItem, aiIndex, akEntry)
EndFunction
bool Function RemoveNthScriptEntry(LeveledItem akLeveledItem, int aiIndex) Global
	return Hydra:Forms:LeveledList.RemoveNthScriptEntry(akLeveledItem, aiIndex)
EndFunction

bool Function AddEditorEntry(LeveledItem akLeveledItem, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.AddEditorEntry(akLeveledItem, akEntry)
EndFunction
bool Function AddScriptEntry(LeveledItem akLeveledItem, LeveledEntry akEntry) Global
	return Hydra:Forms:LeveledList.AddScriptEntry(akLeveledItem, akEntry)
EndFunction

Function AddEditorEntries(LeveledItem akLeveledItem, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.AddEditorEntries(akLeveledItem, akValues)
EndFunction
Function AddScriptEntries(LeveledItem akLeveledItem, LeveledEntry[] akValues) Global
	Hydra:Forms:LeveledList.AddScriptEntries(akLeveledItem, akValues)
EndFunction

Function ClearEntries(LeveledItem akLeveledItem) Global
	Hydra:Forms:LeveledList.ClearEntries(akLeveledItem)
EndFunction
Function ClearEditorEntries(LeveledItem akLeveledItem) Global
	Hydra:Forms:LeveledList.ClearEditorEntries(akLeveledItem)
EndFunction
Function ClearScriptEntries(LeveledItem akLeveledItem) Global
	Hydra:Forms:LeveledList.ClearScriptEntries(akLeveledItem)
EndFunction

KeywordEntry[] Function GetKeywordChances(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetKeywordChances(akLeveledItem)
EndFunction
Function SetKeywordChances(LeveledItem akLeveledItem, KeywordEntry[] akValues) Global
	Hydra:Forms:LeveledList.SetKeywordChances(akLeveledItem, akValues)
EndFunction


bool Function HasFlag(LeveledItem akLeveledItem, int aiFlag) Global
	return Hydra:Forms:LeveledList.HasFlag(akLeveledItem, aiFlag)
EndFunction
int Function GetFlags(LeveledItem akLeveledItem) Global
	return Hydra:Forms:LeveledList.GetFlags(akLeveledItem)
EndFunction
Function SetFlags(LeveledItem akLeveledItem, int aiFlags) Global
	Hydra:Forms:LeveledList.SetFlags(akLeveledItem, aiFlags)
EndFunction
Function SetFlag(LeveledItem akLeveledItem, int aiFlag, bool abSet) Global
	Hydra:Forms:LeveledList.SetFlag(akLeveledItem, aiFlag, abSet)
EndFunction

;/
	Leveled Item Flags:
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
