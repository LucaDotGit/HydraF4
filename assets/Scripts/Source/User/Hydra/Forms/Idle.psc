Scriptname Hydra:Forms:Idle Const Hidden Native

;/
	Provides functions for idle animations.
/;

string Function GetBehaviorGraphName(Idle akIdle) Global Native
Function SetBehaviorGraphName(Idle akIdle, string asValue) Global Native

string Function GetAnimationEventName(Idle akIdle) Global Native
Function SetAnimationEventName(Idle akIdle, string asValue) Global Native

string Function GetAnimationFilePath(Idle akIdle) Global Native
Function SetAnimationFilePath(Idle akIdle, string asValue) Global Native

Idle Function GetParentIdle(Idle akIdle) Global Native
Function SetParentIdle(Idle akIdle, Idle akValue) Global Native

Idle Function GetPreviousIdle(Idle akIdle) Global Native
Function SetPreviousIdle(Idle akIdle, Idle akValue) Global Native

int Function GetLoopMinSeconds(Idle akIdle) Global Native
Function SetLoopMinSeconds(Idle akIdle, int aiValue) Global Native

int Function GetLoopMaxSeconds(Idle akIdle) Global Native
Function SetLoopMaxSeconds(Idle akIdle, int aiValue) Global Native

int Function GetReplayDelay(Idle akIdle) Global Native
Function SetReplayDelay(Idle akIdle, int aiValue) Global Native
