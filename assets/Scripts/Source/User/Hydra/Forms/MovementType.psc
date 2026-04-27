Scriptname Hydra:Forms:MovementType Const Hidden Native

;/
	Provides functions for movement types.
/;

Struct SpeedEntry
	float fStanding
	float fWalking
	float fRunning
	float fSprinting
EndStruct

string Function GetName(MovementType akMovementType) Global Native
Function SetName(MovementType akMovementType, string asValue) Global Native

SpeedEntry Function GetLeftSpeed(MovementType akMovementType) Global Native
Function SetLeftSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetRightSpeed(MovementType akMovementType) Global Native
Function SetRightSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetForwardSpeed(MovementType akMovementType) Global Native
Function SetForwardSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetBackwardSpeed(MovementType akMovementType) Global Native
Function SetBackwardSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetPitchSpeed(MovementType akMovementType) Global Native
Function SetPitchSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetRollSpeed(MovementType akMovementType) Global Native
Function SetRollSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

SpeedEntry Function GetYawSpeed(MovementType akMovementType) Global Native
Function SetYawSpeed(MovementType akMovementType, SpeedEntry akEntry) Global Native

float Function GetFloatHeight(MovementType akMovementType) Global Native
Function SetFloatHeight(MovementType akMovementType, float afValue) Global Native

float Function GetFlightAngleGain(MovementType akMovementType) Global Native
Function SetFlightAngleGain(MovementType akMovementType, float afValue) Global Native
