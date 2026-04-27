Scriptname Hydra:Forms:ConstructibleObject Const Hidden Native

;/
	Provides functions for constructible objects.
/;

Struct ComponentEntry
	Component kComponent
	int iCount
EndStruct

Form Function GetCreatedObject(ConstructibleObject akObject) Global Native
Function SetCreatedObject(ConstructibleObject akObject, Form akValue) Global Native

int Function GetCreatedObjectCount(ConstructibleObject akObject) Global Native
Function SetCreatedObjectCount(ConstructibleObject akObject, int aiValue) Global Native

int Function GetWorkshopPriority(ConstructibleObject akObject) Global Native
Function SetWorkshopPriority(ConstructibleObject akObject, int aiValue) Global Native

Keyword Function GetWorkbenchKeyword(ConstructibleObject akObject) Global Native
Function SetWorkbenchKeyword(ConstructibleObject akObject, Keyword akValue) Global Native

Keyword[] Function GetRecipeFilters(ConstructibleObject akObject) Global Native
Function SetRecipeFilters(ConstructibleObject akObject, Keyword[] akValues) Global Native

ComponentEntry[] Function GetComponents(ConstructibleObject akObject) Global Native
Function SetComponents(ConstructibleObject akObject, ComponentEntry[] akValues) Global Native
