Scriptname Hydra:Forms:Climate Const Hidden Native

;/
	Provides functions for climates.
/;

Struct WeatherEntry
	Weather kWeatherType
	float fChance
	GlobalVariable kChanceGlobal
EndStruct

int Function GetSunriseBegin(Climate akClimate) Global Native
Function SetSunriseBegin(Climate akClimate, int aiValue) Global Native

int Function GetSunriseEnd(Climate akClimate) Global Native
Function SetSunriseEnd(Climate akClimate, int aiValue) Global Native

int Function GetSunsetBegin(Climate akClimate) Global Native
Function SetSunsetBegin(Climate akClimate, int aiValue) Global Native

int Function GetSunsetEnd(Climate akClimate) Global Native
Function SetSunsetEnd(Climate akClimate, int aiValue) Global Native

int Function GetVolatility(Climate akClimate) Global Native
Function SetVolatility(Climate akClimate, int aiValue) Global Native

WeatherEntry[] Function GetWeathers(Climate akClimate) Global Native
Function SetWeathers(Climate akClimate, WeatherEntry[] akValues) Global Native
