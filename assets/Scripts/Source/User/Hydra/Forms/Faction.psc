Scriptname Hydra:Forms:Faction Const Hidden Native

;/
	Provides functions for factions.
/;

Struct RelationEntry
	Faction kFaction
	int iModifier
	int iCombatReaction
EndStruct

ObjectReference Function GetExteriorJailMarker(Faction akFaction) Global Native
Function SetExteriorJailMarker(Faction akFaction, ObjectReference akValue) Global Native

ObjectReference Function GetCompanionWaitMarker(Faction akFaction) Global Native
Function SetCompanionWaitMarker(Faction akFaction, ObjectReference akValue) Global Native

ObjectReference Function GetStolenItemsContainer(Faction akFaction) Global Native
Function SetStolenItemsContainer(Faction akFaction, ObjectReference akValue) Global Native

ObjectReference Function GetPlayerInventoryContainer(Faction akFaction) Global Native
Function SetPlayerInventoryContainer(Faction akFaction, ObjectReference akValue) Global Native

FormList Function GetSharedCrimeFactions(Faction akFaction) Global Native
Function SetSharedCrimeFactions(Faction akFaction, FormList akValue) Global Native

Outfit Function GetJailOutfit(Faction akFaction) Global Native
Function SetJailOutfit(Faction akFaction, Outfit akValue) Global Native

bool Function GetDoesArrest(Faction akFaction) Global Native
Function SetDoesArrest(Faction akFaction, bool abValue) Global Native

bool Function GetDoesAttackOnSight(Faction akFaction) Global Native
Function SetDoesAttackOnSight(Faction akFaction, bool abValue) Global Native

int Function GetMurderValue(Faction akFaction) Global Native
Function SetMurderValue(Faction akFaction, int aiValue) Global Native

int Function GetAssaultValue(Faction akFaction) Global Native
Function SetAssaultValue(Faction akFaction, int aiValue) Global Native

int Function GetTrespassValue(Faction akFaction) Global Native
Function SetTrespassValue(Faction akFaction, int aiValue) Global Native

int Function GetPickpocketValue(Faction akFaction) Global Native
Function SetPickpocketValue(Faction akFaction, int aiValue) Global Native

int Function GetEscapeValue(Faction akFaction) Global Native
Function SetEscapeValue(Faction akFaction, int aiValue) Global Native

float Function GetStealMult(Faction akFaction) Global Native
Function SetStealMult(Faction akFaction, float afValue) Global Native

FormList Function GetMerchantTradeList(Faction akFaction) Global Native
Function SetMerchantTradeList(Faction akFaction, FormList akValue) Global Native

ObjectReference Function GetMerchantContainer(Faction akFaction) Global Native
Function SetMerchantContainer(Faction akFaction, ObjectReference akValue) Global Native

int Function GetMerchantStartHour(Faction akFaction) Global Native
Function SetMerchantStartHour(Faction akFaction, int aiValue) Global Native

int Function GetMerchantEndHour(Faction akFaction) Global Native
Function SetMerchantEndHour(Faction akFaction, int aiValue) Global Native

int Function GetMerchantRadius(Faction akFaction) Global Native
Function SetMerchantRadius(Faction akFaction, int aiValue) Global Native

bool Function GetBuysStolenItems(Faction akFaction) Global Native
Function SetBuysStolenItems(Faction akFaction, bool abValue) Global Native

bool Function GetBuysNonStolenItems(Faction akFaction) Global Native
Function SetBuysNonStolenItems(Faction akFaction, bool abValue) Global Native

bool Function GetTradesEverything(Faction akFaction) Global Native
Function SetTradesEverything(Faction akFaction, bool abValue) Global Native

RelationEntry[] Function GetRelations(Faction akFaction) Global Native
Function SetRelations(Faction akFaction, RelationEntry[] akValues) Global Native

int Function GetCombatReaction_Neutral() Global Native
int Function GetCombatReaction_Enemy() Global Native
int Function GetCombatReaction_Ally() Global Native
int Function GetCombatReaction_Friend() Global Native


bool Function HasFlag(Faction akFaction, int aiFlag) Global Native
int Function GetFlags(Faction akFaction) Global Native
Function SetFlags(Faction akFaction, int aiFlags) Global Native
Function SetFlag(Faction akFaction, int aiFlag, bool abSet) Global Native

int Function GetFlag_IsHiddenFromPlayer() Global Native
int Function GetFlag_IsSpecialInCombat() Global Native
int Function GetFlag_IsPlayerExpelled() Global Native
int Function GetFlag_IsPlayerEnemy() Global Native
int Function GetFlag_TrackCrime() Global Native
int Function GetFlag_IgnoreMurder() Global Native
int Function GetFlag_IgnoreAssault() Global Native
int Function GetFlag_IgnoreStealing() Global Native
int Function GetFlag_IgnoreTrespass() Global Native
int Function GetFlag_DoNotReportMemberCrime() Global Native
int Function GetFlag_UseDefaultCrimeValue() Global Native
int Function GetFlag_IgnorePickpocketing() Global Native
int Function GetFlag_IsMerchant() Global Native
int Function GetFlag_CanBeOwner() Global Native
