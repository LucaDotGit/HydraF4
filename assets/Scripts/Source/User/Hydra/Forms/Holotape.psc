Scriptname Hydra:Forms:Holotape Const Hidden Native

;/
	Provides functions for notes.
/;

bool Function GetHasBeenPlayed(Holotape akHolotape) Global Native
Function SetHasBeenPlayed(Holotape akHolotape, bool abValue) Global Native

int Function GetType(Holotape akHolotape) Global Native

int Function GetType_Voice() Global Native
int Function GetType_Scene() Global Native
int Function GetType_Program() Global Native
int Function GetType_Terminal() Global Native

Sound Function GetVoice(Holotape akHolotape) Global Native
Function SetVoice(Holotape akHolotape, Sound akValue) Global Native

Scene Function GetScene(Holotape akHolotape) Global Native
Function SetScene(Holotape akHolotape, Scene akValue) Global Native

string Function GetProgram(Holotape akHolotape) Global Native
Function SetProgram(Holotape akHolotape, string asValue) Global Native

Terminal Function GetTerminal(Holotape akHolotape) Global Native
Function SetTerminal(Holotape akHolotape, Terminal akValue) Global Native
