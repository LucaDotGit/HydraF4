Scriptname Hydra:Forms:AimModel Const Hidden Native

;/
	Provides functions for aim models.
/;

float Function GetConeMinAngle(AimModel akAimModel) Global Native
Function SetConeMinAngle(AimModel akAimModel, float afValue) Global Native

float Function GetConeMaxAngle(AimModel akAimModel) Global Native
Function SetConeMaxAngle(AimModel akAimModel, float afValue) Global Native

float Function GetConeIncreasePerShot(AimModel akAimModel) Global Native
Function SetConeIncreasePerShot(AimModel akAimModel, float afValue) Global Native

float Function GetConeDecreasePerSec(AimModel akAimModel) Global Native
Function SetConeDecreasePerSec(AimModel akAimModel, float afValue) Global Native

int Function GetConeDecreaseDelayMS(AimModel akAimModel) Global Native
Function SetConeDecreaseDelayMS(AimModel akAimModel, int aiValue) Global Native

float Function GetConeSneakMult(AimModel akAimModel) Global Native
Function SetConeSneakMult(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilArcAngle(AimModel akAimModel) Global Native
Function SetRecoilArcAngle(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilArcRotateAngle(AimModel akAimModel) Global Native
Function SetRecoilArcRotateAngle(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilMaxAnglePerShot(AimModel akAimModel) Global Native
Function SetRecoilMaxAnglePerShot(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilMinAnglePerShot(AimModel akAimModel) Global Native
Function SetRecoilMinAnglePerShot(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilDiminishSpringForce(AimModel akAimModel) Global Native
Function SetRecoilDiminishSpringForce(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilDiminishSightsMult(AimModel akAimModel) Global Native
Function SetRecoilDiminishSightsMult(AimModel akAimModel, float afValue) Global Native

float Function GetRecoilHipMult(AimModel akAimModel) Global Native
Function SetRecoilHipMult(AimModel akAimModel, float afValue) Global Native

int Function GetRunawayRecoilShots(AimModel akAimModel) Global Native
Function SetRunawayRecoilShots(AimModel akAimModel, int aiValue) Global Native

float Function GetBaseStability(AimModel akAimModel) Global Native
Function SetBaseStability(AimModel akAimModel, float afValue) Global Native
