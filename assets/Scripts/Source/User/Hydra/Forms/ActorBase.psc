Scriptname Hydra:Forms:ActorBase Const Hidden Native

;/
	Provides functions for base actors.
/;

Import Hydra:Colors
Import Hydra:Vectors3

Struct FactionRank
	Faction kFaction
	int iRank
EndStruct

Struct PerkRank
	Perk kPerk
	int iRank
EndStruct

string Function GetShortName(ActorBase akActorBase) Global Native
Function SetShortName(ActorBase akActorBase, string acsValue) Global Native

bool Function GetIsFemale(ActorBase akActorBase) Global Native
Function SetIsFemale(ActorBase akActorBase, bool abValue) Global Native

float Function GetMinHeight(ActorBase akActorBase) Global Native
Function SetMinHeight(ActorBase akActorBase, float afValue) Global Native

float Function GetMaxHeight(ActorBase akActorBase) Global Native
Function SetMaxHeight(ActorBase akActorBase, float afValue) Global Native

VoiceType Function GetVoiceType(ActorBase akActorBase) Global Native
Function SetVoiceType(ActorBase akActorBase, VoiceType akValue) Global Native

Vector3 Function GetWeight(ActorBase akActorBase) Global Native
Function SetWeight(ActorBase akActorBase, Vector3 akValue) Global Native

LeveledItem Function GetDeathItem(ActorBase akActorBase) Global Native
Function SetDeathItem(ActorBase akActorBase, LeveledItem akValue) Global Native

int Function GetLevel(ActorBase akActorBase) Global Native
Function SetLevel(ActorBase akActorBase, int aiValue) Global Native

int Function GetMinLevel(ActorBase akActorBase) Global Native
Function SetMinLevel(ActorBase akActorBase, int aiValue) Global Native

int Function GetMaxLevel(ActorBase akActorBase) Global Native
Function SetMaxLevel(ActorBase akActorBase, int aiValue) Global Native

int Function GetXPValueOffset(ActorBase akActorBase) Global Native
Function SetXPValueOffset(ActorBase akActorBase, int aiValue) Global Native

int Function GetBleedoutOverride(ActorBase akActorBase) Global Native
Function SetBleedoutOverride(ActorBase akActorBase, int aiValue) Global Native

Class Function GetClass(ActorBase akActorBase) Global Native
Function SetClass(ActorBase akActorBase, Class akValue) Global Native

Form[] Function GetTemplates(ActorBase akActorBase) Global Native
Function SetTemplates(ActorBase akActorBase, Form[] akValues) Global Native

Form Function GetTemplateByFlag(ActorBase akActorBase, int aiFlag) Global Native
Function SetTemplateByFlag(ActorBase akActorBase, int aiFlag, Form akValue) Global Native

Form Function GetTraitsTemplate(ActorBase akActorBase) Global Native
Function SetTraitsTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetStatsTemplate(ActorBase akActorBase) Global Native
Function SetStatsTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetFactionsTemplate(ActorBase akActorBase) Global Native
Function SetFactionsTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetSpellsTemplate(ActorBase akActorBase) Global Native
Function SetSpellsTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetAIDataTemplate(ActorBase akActorBase) Global Native
Function SetAIDataTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetAIPackagesTemplate(ActorBase akActorBase) Global Native
Function SetAIPackagesTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetBaseDataTemplate(ActorBase akActorBase) Global Native
Function SetBaseDataTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetInventoryTemplate(ActorBase akActorBase) Global Native
Function SetInventoryTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetScriptTemplate(ActorBase akActorBase) Global Native
Function SetScriptTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetAIDefaultPackageListTemplate(ActorBase akActorBase) Global Native
Function SetAIDefaultPackageListTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetAttackDataTemplate(ActorBase akActorBase) Global Native
Function SetAttackDataTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetKeywordsTemplate(ActorBase akActorBase) Global Native
Function SetKeywordsTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetDefaultTemplate(ActorBase akActorBase) Global Native
Function SetDefaultTemplate(ActorBase akActorBase, Form akValue) Global Native

Form Function GetLegendaryTemplate(ActorBase akActorBase) Global Native
Function SetLegendaryTemplate(ActorBase akActorBase, Form akValue) Global Native

GlobalVariable Function GetLegendaryChanceGlobal(ActorBase akActorBase) Global Native
Function SetLegendaryChanceGlobal(ActorBase akActorBase, GlobalVariable akValue) Global Native

FactionRank[] Function GetFactions(ActorBase akActorBase) Global Native
Function SetFactions(ActorBase akActorBase, FactionRank[] akValues) Global Native

Faction Function GetCrimeFaction(ActorBase akActorBase) Global Native
Function SetCrimeFaction(ActorBase akActorBase, Faction akValue) Global Native

Relationship[] Function GetRelationships(ActorBase akActorBase) Global Native
Function SetRelationships(ActorBase akActorBase, Relationship[] akValues) Global Native

CombatStyle Function GetCombatStyle(ActorBase akActorBase) Global Native
Function SetCombatStyle(ActorBase akActorBase, CombatStyle akValue) Global Native

Package[] Function GetAIPackages(ActorBase akActorBase) Global Native
Function SetAIPackages(ActorBase akActorBase, Package[] akValues) Global Native

FormList Function GetDefaultPackageList(ActorBase akActorBase) Global Native
Function SetDefaultPackageList(ActorBase akActorBase, FormList akValue) Global Native

FormList Function GetSpectatorOverridePackageList(ActorBase akActorBase) Global Native
Function SetSpectatorOverridePackageList(ActorBase akActorBase, FormList akValue) Global Native

FormList Function GetObserveCorpseOverridePackageList(ActorBase akActorBase) Global Native
Function SetObserveCorpseOverridePackageList(ActorBase akActorBase, FormList akValue) Global Native

FormList Function GetGuardWarnOverridePackageList(ActorBase akActorBase) Global Native
Function SetGuardWarnOverridePackageList(ActorBase akActorBase, FormList akValue) Global Native

FormList Function GetCombatOverridePackageList(ActorBase akActorBase) Global Native
Function SetCombatOverridePackageList(ActorBase akActorBase, FormList akValue) Global Native

Outfit Function GetDefaultOutfit(ActorBase akActorBase) Global Native
Function SetDefaultOutfit(ActorBase akActorBase, Outfit akValue) Global Native

Outfit Function GetSleepOutfit(ActorBase akActorBase) Global Native
Function SetSleepOutfit(ActorBase akActorBase, Outfit akValue) Global Native

Furniture Function GetPowerArmorFurniture(ActorBase akActorBase) Global Native
Function SetPowerArmorFurniture(ActorBase akActorBase, Furniture akValue) Global Native

Hydra:Forms:Form:ContainerEntry[] Function GetInventoryItems(ActorBase akActorBase) Global
	return Hydra:Forms:Form.GetContainerItems(akActorBase)
EndFunction
Function SetInventoryItems(ActorBase akActorBase, Hydra:Forms:Form:ContainerEntry[] akValues) Global
	Hydra:Forms:Form.SetContainerItems(akActorBase, akValues)
EndFunction

Spell[] Function GetSpells(ActorBase akActorBase) Global Native
Function SetSpells(ActorBase akActorBase, Spell[] akValues) Global Native

PerkRank[] Function GetPerks(ActorBase akActorBase) Global Native
Function SetPerks(ActorBase akActorBase, PerkRank[] akValues) Global Native

ColorForm Function GetHairColor(ActorBase akActorBase) Global Native
Function SetHairColor(ActorBase akActorBase, ColorForm akValue) Global Native

ColorForm Function GetFacialHairColor(ActorBase akActorBase) Global Native
Function SetFacialHairColor(ActorBase akActorBase, ColorForm akValue) Global Native

TextureSet Function GetFaceDetails(ActorBase akActorBase) Global Native
Function SetFaceDetails(ActorBase akActorBase, TextureSet akValue) Global Native

Color Function GetBodyTintColor(ActorBase akActorBase) Global Native
Function SetBodyTintColor(ActorBase akActorBase, Color akValue) Global Native

HeadPart[] Function GetHeadParts(ActorBase akActorBase) Global Native
Function SetHeadParts(ActorBase akActorBase, HeadPart[] akValues) Global Native

ActorBase Function GetRootFaceActorBase(ActorBase akActorBase) Global Native


bool Function HasFlag(ActorBase akActorBase, int aiFlag) Global Native
int Function GetFlags(ActorBase akActorBase) Global Native
Function SetFlags(ActorBase akActorBase, int aiValue) Global Native
Function SetFlag(ActorBase akActorBase, int aiFlag, bool abSet) Global Native

bool Function HasTemplateFlag(ActorBase akActorBase, int aiFlag) Global Native
int Function GetTemplateFlags(ActorBase akActorBase) Global Native
Function SetTemplateFlags(ActorBase akActorBase, int aiValue) Global Native
Function SetTemplateFlag(ActorBase akActorBase, int aiFlag, bool abSet) Global Native

;/
	Actor Base Flags:
/;

int Function GetFlag_IsFemale() Global Native
int Function GetFlag_IsEssential() Global Native
int Function GetFlag_IsCharGenFacePreset() Global Native
int Function GetFlag_Respawn() Global Native
int Function GetFlag_AutoCalcStats() Global Native
int Function GetFlag_IsUnique() Global Native
int Function GetFlag_DoNotAffectStealthMeter() Global Native
int Function GetFlag_HasPlayerLevelMult() Global Native
int Function GetFlag_UseTemplates() Global Native
int Function GetFlag_IsProtected() Global Native
int Function GetFlag_IsSummonable() Global Native
int Function GetFlag_DoNotBleed() Global Native
int Function GetFlag_HasBleedoutOverride() Global Native
int Function GetFlag_UseOppositeGenderAnims() Global Native
int Function GetFlag_IsSimpleActor() Global Native
int Function GetFlag_HasLoopedScript() Global Native
int Function GetFlag_HasLoopedAudio() Global Native
int Function GetFlag_IsGhost() Global Native
int Function GetFlag_IsInvulnerable() Global Native

;/
	Template Flags:
/;

int Function GetTemplateFlag_UseTraits() Global Native
int Function GetTemplateFlag_UseStats() Global Native
int Function GetTemplateFlag_UseFactions() Global Native
int Function GetTemplateFlag_UseSpells() Global Native
int Function GetTemplateFlag_UseAIData() Global Native
int Function GetTemplateFlag_UseAIPackages() Global Native
int Function GetTemplateFlag_UseBaseData() Global Native
int Function GetTemplateFlag_UseInventory() Global Native
int Function GetTemplateFlag_UseScript() Global Native
int Function GetTemplateFlag_UseAIDefaultPackageList() Global Native
int Function GetTemplateFlag_UseAttackData() Global Native
int Function GetTemplateFlag_UseKeywords() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_IsCompressed() Global Native
int Function GetRecordFlag_HasBleedoutOverride() Global Native
