Scriptname Hydra:VarPairs Const Hidden Native

;/
	Provides a data structure for var pairs.
/;

Struct VarPair
	Var vKey
	Var vValue
EndStruct

VarPair Function Create(Var avKey, Var avValue) Global
	VarPair kPair = new VarPair
	kPair.vKey = avKey
	kPair.vValue = avValue
	return kPair
EndFunction

int Function Compare(VarPair akLeft, VarPair akRight) Global Native
bool Function Equals(VarPair akLeft, VarPair akRight) Global Native
