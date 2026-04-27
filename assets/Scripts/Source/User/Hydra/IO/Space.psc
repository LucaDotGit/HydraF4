Scriptname Hydra:IO:Space Const Hidden Native

;/
	Provides functions for retrieving space information.

	Notes:
	- These functions are only valid within the game's root- and its sub-directories.
/;

Import Hydra:Int64

Struct SpaceInfo
	Long lTotalSpace
	Long lFreeSpace
	Long lAvailableSpace
EndStruct

SpaceInfo Function GetCurrentSpaceInfo() Global Native
