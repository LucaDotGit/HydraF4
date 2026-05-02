Scriptname Hydra:Forms:MiscObject Const Hidden Native

;/
	Provides functions for misc items.
/;

Struct ComponentEntry
	Component kComponent
	int iCount = 1
EndStruct

ComponentEntry[] Function GetComponents(MiscObject akMiscObject) Global Native
Function SetComponents(MiscObject akMiscObject, ComponentEntry[] akValues) Global Native
