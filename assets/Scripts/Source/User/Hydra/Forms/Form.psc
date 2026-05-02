Scriptname Hydra:Forms:Form Const Hidden Native

;/
	Provides functions for forms, the base type of all game objects.

	Notes:
	- Any created or copied form is not stored in saves and persists only for the current game session.
	- The `Var[]` filters support the following types:
	  - form types (string)
	  - plugin names (string)
	  - references
	  - base objects
	  - keywords
	  - factions
	  - races
	  - form lists (recursive)
	- For a list of all form types,
	  see: https://github.com/libxse/commonlibf4/blob/main/include/RE/E/ENUM_FORM_ID.h.
	- For a list of all form flags,
	  see: https://github.com/powerof3/CommonLibSSE/blob/e3626d228d60b92a82410accd475bffdd7245653/include/RE/T/TESForm.h#L51.
	- For a list of all reference flags,
	  see: https://github.com/powerof3/CommonLibSSE/blob/e3626d228d60b92a82410accd475bffdd7245653/include/RE/T/TESObjectREFR.h#L146.
/;

Struct ContainerEntry
	Form kItem
	int iCount = 1
	Form kOwner
	int iOwnerRank = 0
EndStruct

Struct PropertyEntry
	ActorValue kValueHolder
	float iValue = 0.0
EndStruct

; The output may be `none` if no form factory is available for a type.
Form Function Create(int aiType) Global Native
Form Function CreateAndCopy(Form akForm) Global Native
bool Function CopyTo(Form akSource, Form akTarget) Global Native

bool Function HasFlag(Form akForm, int aiFlag) Global Native
int Function GetFlags(Form akForm) Global Native
Function SetFlags(Form akForm, int aiFlags) Global Native
Function SetFlag(Form akForm, int aiFlag, bool abSet) Global Native

int Function GetRecordFlag_IsDeleted() Global Native
int Function GetRecordFlag_IsIgnored() Global Native

bool Function GetIsDeleted(Form akForm) Global Native
Function SetIsDeleted(Form akForm, bool abValue) Global Native

bool Function GetIsIgnored(Form akForm) Global Native
Function SetIsIgnored(Form akForm, bool abValue) Global Native

bool Function GetIsCreated(Form akForm) Global Native

int Function GetFormId(Form akForm) Global Native
int Function GetLocalFormId(Form akForm) Global Native

string Function GetEditorId(Form akForm) Global Native
bool Function SetEditorId(Form akForm, string asEditorId) Global Native

string Function GetScriptName(Form akForm) Global Native

int Function GetTypeId(Form akForm) Global Native
int Function GetTypeIdByString(string asTypeString) Global Native
string Function GetTypeString(Form akForm) Global Native
string Function GetTypeStringById(int aiTypeId) Global Native

bool Function IsInPlugin(Form akForm, string asPluginName) Global Native
int Function GetPluginCount(Form akForm) Global Native
int Function GetPluginIndexByName(Form akForm, string asPluginName) Global Native
; Index >= 0 returns the nth plugin name while index < 0 returns the last plugin name.
string Function GetNthPluginName(Form akForm, int aiIndex) Global Native

Form[] Function GetAllByType(int aiType, string asPluginName = "") Global Native
Form[] Function FindAllByType(Form[] akForms, int aiType) Global Native
Form[] Function FindAllByPluginName(Form[] akForms, string asPluginName) Global Native
Form[] Function FindAllByFlag(Form[] akForms, int aiFlag, bool abIsSet) Global Native

bool Function ContainsAnyByFilter(Form[] akForms, Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native
bool Function ContainsAllByFilter(Form[] akForms, Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native
Form[] Function FindAllByFilter(Form[] akForms, Var[] akIncludedObjects = none, Var[] akExcludedObjects = none) Global Native

bool Function ContainsKeyword(Form akForm, Keyword akKeyword) Global Native
bool Function ContainsAnyKeyword(Form akForm, Keyword[] akKeywords) Global Native
bool Function ContainsAllKeywords(Form akForm, Keyword[] akKeywords) Global Native

bool Function ContainsPartialKeywordString(Form akForm, string asKeywordId) Global Native
bool Function ContainsExactKeywordString(Form akForm, string asKeywordId) Global Native

bool Function ContainsAnyPartialKeywordString(Form akForm, string[] akKeywordIds) Global Native
bool Function ContainsAllPartialKeywordStrings(Form akForm, string[] akKeywordIds) Global Native
bool Function ContainsAnyExactKeywordString(Form akForm, string[] akKeywordIds) Global Native
bool Function ContainsAllExactKeywordStrings(Form akForm, string[] akKeywordIds) Global Native

int Function GetKeywordCount(Form akForm) Global Native
int Function GetKeywordIndex(Form akForm, Keyword akKeyword) Global Native

Keyword[] Function GetKeywords(Form akForm) Global Native
bool Function SetKeywords(Form akForm, Keyword[] akKeywords) Global Native

Keyword Function GetNthKeyword(Form akForm, int aiIndex) Global Native
bool Function SetNthKeyword(Form akForm, int aiIndex, Keyword akKeyword) Global Native
bool Function RemoveNthKeyword(Form akForm, int aiIndex) Global Native

bool Function AddKeyword(Form akForm, Keyword akKeyword) Global Native
bool Function RemoveKeyword(Form akForm, Keyword akKeyword) Global Native
bool Function ReplaceKeyword(Form akForm, Keyword akOldKeyword, Keyword akNewKeyword) Global Native

bool Function AddKeywords(Form akForm, Keyword[] akKeywords) Global Native
bool Function RemoveKeywords(Form akForm, Keyword[] akKeywords) Global Native

bool Function ClearKeywords(Form akForm) Global Native

; For: Too many to list
string Function GetName(Form akForm) Global Native
bool Function SetName(Form akForm, string acsValue) Global Native

; For:	ActorValue, Ammo, Armor, Book, Class, CollisionLayer, ConstructibleObject, Message, ObjectMod,
; 		Perk, Potion, Race, Shout, Spell, Weapon
string Function GetDescription(Form akForm) Global Native

; For:	Ammo, Book, Component, Holotape, Ingredient, Light, MiscObject, Scroll
int Function GetValue(Form akForm) Global Native
bool Function SetValue(Form akForm, int aiValue) Global Native

; For:	Ammo, Book, Container, Holotape, Ingredient, Light, MiscObject, Potion, Scroll
float Function GetWeight(Form akForm) Global Native
bool Function SetWeight(Form akForm, float afValue) Global Native

; For:	Armor, ArmorAddon, ActorBase
Race Function GetRace(Form akForm) Global Native
bool Function SetRace(Form akForm, Race akValue) Global Native

; For:	ActorBase, Race
Armor Function GetSkin(Form akForm) Global Native
bool Function SetSkin(Form akForm, Armor akValue) Global Native

; For:	Armor, Ingredient, Light, Potion, Shout, Spell, Weapon
EquipSlot Function GetEquipType(Form akForm) Global Native
bool Function SetEquipType(Form akForm, EquipSlot akValue) Global Native

; For: Armor, Explosion, Weapon
Enchantment Function GetEnchantment(Form akForm) Global Native
bool Function SetEnchantment(Form akForm, Enchantment akValue) Global Native

; For:	Armor, Weapon
InstanceNamingRules Function GetInstanceNamingRules(Form akForm) Global Native
bool Function SetInstanceNamingRules(Form akForm, InstanceNamingRules akValue) Global Native

; For:	Armor, Weapon
ImpactDataSet Function GetBlockBashImpactDataSet(Form akForm) Global Native
bool Function SetBlockBashImpactDataSet(Form akForm, ImpactDataSet akValue) Global Native
MaterialType Function GetBlockBashAltMaterialType(Form akForm) Global Native
bool Function SetBlockBashAltMaterialType(Form akForm, MaterialType akValue) Global Native

; For:	Ammo, Armor, Book, ConstructibleObject, Holotape, Ingredient, MiscObject, Potion, Scroll, Weapon
Sound Function GetPickUpSound(Form akForm) Global Native
bool Function SetPickUpSound(Form akForm, Sound akValue) Global Native
Sound Function GetPutDownSound(Form akForm) Global Native
bool Function SetPutDownSound(Form akForm, Sound akValue) Global Native

; For:	Component, Potion
Sound Function GetCraftingSound(Form akForm) Global Native
bool Function SetCraftingSound(Form akForm, Sound akValue) Global Native

; For:	Activator, Container, Door, ActorBase
Terminal Function GetTerminal(Form akForm) Global Native
bool Function SetTerminal(Form akForm, Terminal akValue) Global Native

; For:	Activator, ActorBase, Container, Static
LocationRefType Function GetForcedLocationRefType(Form akForm) Global Native
bool Function SetForcedLocationRefType(Form akForm, LocationRefType akValue) Global Native

; For:	Activator, AddonNode, Ammo, AnimObject, ArtObject, Book, Container, Door, HeadPart, Ingredient,
; 		LeveledActor, Light, MiscObject, ObjectMod, Potion, Scroll, Static, StaticCollection, Tree, Weapon
MatSwap Function GetMaterialSwap(Form akForm) Global Native
bool Function SetMaterialSwap(Form akForm, MatSwap akValue) Global Native
float Function GetMaterialSwapColorRemapIndex(Form akForm) Global Native
bool Function SetMaterialSwapColorRemapIndex(Form akForm, float afValue) Global Native

; For:	ActorBase, Container
ContainerEntry[] Function GetContainerItems(Form akForm) Global Native
bool Function SetContainerItems(Form akForm, ContainerEntry[] akValues) Global Native

; For:	Activator, ActorBase, Class, Container, Light, Race, Static
PropertyEntry[] Function GetProperties(Form akForm) Global Native
bool Function SetProperties(Form akForm, PropertyEntry[] akProperties) Global Native
