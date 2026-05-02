Scriptname Hydra:Forms:ColorForm Const Hidden Native

;/
	Provides functions for color forms.
/;

Import Hydra:Colors

Color Function GetColor(ColorForm akColorForm) Global Native
Function SetColor(ColorForm akColorForm, Color akValue) Global Native

float Function GetRemappingIndex(ColorForm akColorForm) Global Native
Function SetRemappingIndex(ColorForm akColorForm, float afValue) Global Native


bool Function HasFlag(ColorForm akColorForm, int aiFlag) Global Native
int Function GetFlags(ColorForm akColorForm) Global Native
Function SetFlags(ColorForm akColorForm, int aiFlags) Global Native
Function SetFlag(ColorForm akColorForm, int aiFlag, bool abSet) Global Native

;/
	Color Form Flags:
/;

int Function GetFlag_IsPlayable() Global Native
int Function GetFlag_HasRemappingIndex() Global Native
int Function GetFlag_HasExtendedLut() Global Native

;/
	Record/Form Flags:
/;

int Function GetRecordFlag_IsNonPlayable() Global Native
