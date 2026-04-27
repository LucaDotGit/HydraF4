Scriptname Hydra:Colors Const Hidden Native

;/
	Provides a data structure for colors.
/;

Import Hydra:Operator

Struct Color
	int iRed
	int iGreen
	int iBlue
	int iAlpha
EndStruct

Color Function Create(int aiRed = 0, int aiGreen = 0, int aiBlue = 0, int aiAlpha = 0) Global
	Color kColor = new Color
	kColor.iRed = aiRed
	kColor.iGreen = aiGreen
	kColor.iBlue = aiBlue
	kColor.iAlpha = aiAlpha
	return kColor
EndFunction

Color Function FromHex(int aiValue) Global Native
int Function ToHex(Color akColor) Global Native

Color Function FromString(string asValue) Global Native
string Function ToString(Color akColor) Global Native
