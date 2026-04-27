Scriptname Hydra:Forms:DamageType Const Hidden Native

;/
	Provides functions for damage types.
/;

ActorValue Function GetResistance(DamageType akDamageType) Global Native
Function SetResistance(DamageType akDamageType, ActorValue akValue) Global Native

Spell Function GetSpell(DamageType akDamageType) Global Native
Function SetSpell(DamageType akDamageType, Spell akValue) Global Native
