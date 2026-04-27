Scriptname Hydra:Forms:Perk Const Hidden Native

;/
	Provides functions for perks.

	Notes:
	- The entries are sorted by their priority.
/;

Struct Entry
	int iRank
	int iPriority
EndStruct

Struct QuestEntry
	Quest kQuest
	int iStage
EndStruct

Struct ValueTwoEntry
	float fValue01
	float fValue02
EndStruct

Struct ActorValueEntry
	ActorValue kActorValue
	float fValue
EndStruct

bool Function GetIsTrait(Perk akPerk) Global Native
Function SetIsTrait(Perk akPerk, bool abValue) Global Native

bool Function GetIsPlayable(Perk akPerk) Global Native
Function SetIsPlayable(Perk akPerk, bool abValue) Global Native

bool Function GetIsHidden(Perk akPerk) Global Native
Function SetIsHidden(Perk akPerk, bool abValue) Global Native

int Function GetLevel(Perk akPerk) Global Native
Function SetLevel(Perk akPerk, int aiValue) Global Native

int Function GetRankCount(Perk akPerk) Global Native
Function SetRankCount(Perk akPerk, int aiValue) Global Native

Perk Function GetNextPerk(Perk akPerk) Global Native
Function SetNextPerk(Perk akPerk, Perk akValue) Global Native

Sound Function GetSound(Perk akPerk) Global Native
Function SetSound(Perk akPerk, Sound akValue) Global Native

string Function GetUIFilePath(Perk akPerk) Global Native
Function SetUIFilePath(Perk akPerk, string asValue) Global Native

Entry[] Function GetEntries(Perk akPerk) Global Native
Function SetEntries(Perk akPerk, Entry[] akValues) Global Native

int Function GetNthEntryType(Perk akPerk, int aiIndex) Global Native
int Function GetNthEntryFunctionType(Perk akPerk, int aiIndex) Global Native

int Function GetEntryType_Invalid() Global Native
int Function GetEntryType_Quest() Global Native
int Function GetEntryType_Ability() Global Native
int Function GetEntryType_EntryPoint() Global Native

int Function GetEntryFunctionType_Invalid() Global Native
int Function GetEntryFunctionType_OneValue() Global Native
int Function GetEntryFunctionType_TwoValue() Global Native
int Function GetEntryFunctionType_LeveledList() Global Native
int Function GetEntryFunctionType_ActivateChoice() Global Native
int Function GetEntryFunctionType_Spell() Global Native
int Function GetEntryFunctionType_BooleanGraphVariable() Global Native
int Function GetEntryFunctionType_Text() Global Native
int Function GetEntryFunctionType_ActorValue() Global Native

QuestEntry Function GetNthQuestEntry(Perk akPerk, int aiIndex) Global Native
bool Function SetNthQuestEntry(Perk akPerk, int aiIndex, QuestEntry akEntry) Global Native

float Function GetNthEntryValue(Perk akPerk, int aiEntryIndex) Global Native
bool Function SetNthEntryValue(Perk akPerk, int aiEntryIndex, float afValue) Global Native

ValueTwoEntry Function GetNthValueTwoEntry(Perk akPerk, int aiIndex) Global Native
bool Function SetNthValueTwoEntry(Perk akPerk, int aiIndex, ValueTwoEntry akEntry) Global Native

LeveledItem Function GetNthEntryLeveledList(Perk akPerk, int aiIndex) Global Native
bool Function SetNthEntryLeveledList(Perk akPerk, int aiIndex, LeveledItem akLeveledList) Global Native

Spell Function GetNthEntrySpell(Perk akPerk, int aiIndex) Global Native
bool Function SetNthEntrySpell(Perk akPerk, int aiIndex, Spell akSpell) Global Native

string Function GetNthEntryText(Perk akPerk, int aiIndex) Global Native
bool Function SetNthEntryText(Perk akPerk, int aiIndex, string asText) Global Native

ActorValueEntry Function GetNthActorValueEntry(Perk akPerk, int aiIndex) Global Native
bool Function SetNthActorValueEntry(Perk akPerk, int aiIndex, ActorValueEntry akEntry) Global Native
