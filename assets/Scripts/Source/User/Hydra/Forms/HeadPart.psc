Scriptname Hydra:Forms:HeadPart Const Hidden Native

;/
	Provides functions for head parts.
/;

int Function GetType(HeadPart akHeadPart) Global Native
Function SetType(HeadPart akHeadPart, int aiValue) Global Native

int Function GetType_Misc() Global Native
int Function GetType_Face() Global Native
int Function GetType_Eyes() Global Native
int Function GetType_Hair() Global Native
int Function GetType_FacialHair() Global Native
int Function GetType_Scar() Global Native
int Function GetType_Eyebrows() Global Native
int Function GetType_Meatcaps() Global Native
int Function GetType_Teeth() Global Native
int Function GetType_HeadRear() Global Native

HeadPart[] Function GetExtraParts(HeadPart akHeadPart) Global Native
Function SetExtraParts(HeadPart akHeadPart, HeadPart[] akValues) Global Native

TextureSet Function GetTextureSet(HeadPart akHeadPart) Global Native
Function SetTextureSet(HeadPart akHeadPart, TextureSet akValue) Global Native

ColorForm Function GetColorForm(HeadPart akHeadPart) Global Native
Function SetColorForm(HeadPart akHeadPart, ColorForm akValue) Global Native

FormList Function GetValidRaceList(HeadPart akHeadPart) Global Native
Function SetValidRaceList(HeadPart akHeadPart, FormList akValue) Global Native


bool Function HasFlag(HeadPart akHeadPart, int aiFlag) Global Native
int Function GetFlags(HeadPart akHeadPart) Global Native
Function SetFlags(HeadPart akHeadPart, int aiFlags) Global Native
Function SetFlag(HeadPart akHeadPart, int aiFlag, bool abSet) Global Native

;/
	Head Part Flags:
/;

int Function GetFlag_IsPlayable() Global Native
int Function GetFlag_CannotBeMale() Global Native
int Function GetFlag_CannotBeFemale() Global Native
int Function GetFlag_IsExtraPart() Global Native
int Function GetFlag_UseSolidTint() Global Native
int Function GetFlag_UseTextureTint() Global Native
