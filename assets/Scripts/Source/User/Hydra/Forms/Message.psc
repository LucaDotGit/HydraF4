Scriptname Hydra:Forms:Message Const Hidden Native

;/
	Provides functions for messages.
/;

Quest Function GetOwnerQuest(Message akMessage) Global Native
Function SetOwnerQuest(Message akMessage, Quest akValue) Global Native

int Function GetDisplayTime(Message akMessage) Global Native
Function SetDisplayTime(Message akMessage, int aiValue) Global Native

string Function GetUIFilePath(Message akMessage) Global Native
Function SetUIFilePath(Message akMessage, string asValue) Global Native

string Function GetShortName(Message akMessage) Global Native
Function SetShortName(Message akMessage, string acsValue) Global Native

int Function GetButtonCount(Message akMessage) Global Native

string Function GetNthButtonText(Message akMessage, int aiIndex) Global Native
bool Function SetNthButtonText(Message akMessage, int aiIndex, string acsValue) Global Native


bool Function HasFlag(Message akMessage, int aiFlag) Global Native
int Function GetFlags(Message akMessage) Global Native
Function SetFlags(Message akMessage, int aiFlags) Global Native
Function SetFlag(Message akMessage, int aiFlag, bool abSet) Global Native

;/
	Message Box Flags:
/;

int Function GetFlag_IsMessageBox() Global Native
int Function GetFlag_HasInitialDelay() Global Native
