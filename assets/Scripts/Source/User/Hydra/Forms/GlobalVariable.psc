Scriptname Hydra:Forms:GlobalVariable Const Hidden Native

;/
	Provides functions for global variables.

	Notes:
	- The "Force*" functions ignore the constant flag.
/;

float Function GetValue(GlobalVariable akGlobal) Global Native
bool Function SetValue(GlobalVariable akGlobal, float afValue) Global Native
bool Function ModValue(GlobalVariable akGlobal, float afValue) Global Native

Function ForceSetValue(GlobalVariable akGlobal, float afValue) Global Native
Function ForceModValue(GlobalVariable akGlobal, float afValue) Global Native


;/
	Record/Form Flags:
/;

int Function GetRecordFlag_IsConstant() Global Native
