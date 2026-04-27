Scriptname Hydra:Forms:Flora Const Hidden Native

;/
	Provides functions for flora.
/;

Form Function GetHarvestItem(Flora akFlora) Global Native
Function SetHarvestItem(Flora akFlora, Form akValue) Global Native

Sound Function GetHarvestSound(Flora akFlora) Global Native
Function SetHarvestSound(Flora akFlora, Sound akValue) Global Native

int Function GetSpringHarvestChance(Flora akFlora) Global Native
Function SetSpringHarvestChance(Flora akFlora, int aiValue) Global Native

int Function GetSummerHarvestChance(Flora akFlora) Global Native
Function SetSummerHarvestChance(Flora akFlora, int aiValue) Global Native

int Function GetFallHarvestChance(Flora akFlora) Global Native
Function SetFallHarvestChance(Flora akFlora, int aiValue) Global Native

int Function GetWinterHarvestChance(Flora akFlora) Global Native
Function SetWinterHarvestChance(Flora akFlora, int aiValue) Global Native
