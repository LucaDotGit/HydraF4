Scriptname Hydra:Forms:LeveledList Const Hidden Native

;/
	Provides shared data for leveled items and actors.

	Notes:
	- Leveled lists only support up to 255 entries.
/;

Struct LeveledEntry
	Form kObject
	int iCount
	int iLevel
	int iChanceNone
EndStruct

Struct KeywordEntry
	Keyword kKeyword
	int iChance
EndStruct

int Function GetMaxUseAllCount(Form akLeveledList) Global Native
bool Function SetMaxUseAllCount(Form akLeveledList, int aiValue) Global Native

int Function GetChanceNone(Form akLeveledList) Global Native
bool Function SetChanceNone(Form akLeveledList, int aiValue) Global Native

GlobalVariable Function GetChanceNoneGlobal(Form akLeveledList) Global Native
bool Function SetChanceNoneGlobal(Form akLeveledList, GlobalVariable akValue) Global Native

int Function GetEntryCount(Form akLeveledList) Global Native
int Function GetEditorEntryCount(Form akLeveledList) Global Native
int Function GetScriptEntryCount(Form akLeveledList) Global Native

LeveledEntry[] Function GetAllEntries(Form akLeveledList) Global Native

LeveledEntry[] Function GetEditorEntries(Form akLeveledList) Global Native
bool Function SetEditorEntries(Form akLeveledList, LeveledEntry[] akValues) Global Native

LeveledEntry[] Function GetScriptEntries(Form akLeveledList) Global Native
bool Function SetScriptEntries(Form akLeveledList, LeveledEntry[] akValues) Global Native

LeveledEntry Function GetNthEntry(Form akLeveledList, int aiIndex) Global Native
bool Function SetNthEntry(Form akLeveledList, int aiIndex, LeveledEntry akEntry) Global Native
bool Function RemoveNthEntry(Form akLeveledList, int aiIndex) Global Native

LeveledEntry Function GetNthEditorEntry(Form akLeveledList, int aiIndex) Global Native
bool Function SetNthEditorEntry(Form akLeveledList, int aiIndex, LeveledEntry akEntry) Global Native
bool Function RemoveNthEditorEntry(Form akLeveledList, int aiIndex) Global Native

LeveledEntry Function GetNthScriptEntry(Form akLeveledList, int aiIndex) Global Native
bool Function SetNthScriptEntry(Form akLeveledList, int aiIndex, LeveledEntry akEntry) Global Native
bool Function RemoveNthScriptEntry(Form akLeveledList, int aiIndex) Global Native

bool Function AddEditorEntry(Form akLeveledList, LeveledEntry akEntry) Global Native
bool Function AddScriptEntry(Form akLeveledList, LeveledEntry akEntry) Global Native

bool Function AddEditorEntries(Form akLeveledList, LeveledEntry[] akValues) Global Native
bool Function AddScriptEntries(Form akLeveledList, LeveledEntry[] akValues) Global Native

bool Function ClearEntries(Form akLeveledList) Global Native
bool Function ClearEditorEntries(Form akLeveledList) Global Native
bool Function ClearScriptEntries(Form akLeveledList) Global Native

KeywordEntry[] Function GetKeywordChances(Form akLeveledList) Global Native
bool Function SetKeywordChances(Form akLeveledList, KeywordEntry[] akValues) Global Native


bool Function HasFlag(Form akLeveledList, int aiFlag) Global Native
int Function GetFlags(Form akLeveledList) Global Native
bool Function SetFlags(Form akLeveledList, int aiFlags) Global Native
bool Function SetFlag(Form akLeveledList, int aiFlag, bool abSet) Global Native

;/
	Leveled List Flags:
/;

int Function GetFlag_CalculateAllLessEqualsPlayerLevel() Global Native
int Function GetFlag_CalculateEachItemInCount() Global Native
int Function GetFlag_UseAll() Global Native
