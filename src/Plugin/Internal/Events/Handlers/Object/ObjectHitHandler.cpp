#include "Plugin/Internal/Events/Handlers/Object/ObjectHitHandler.hpp"

namespace Plugin::Internal::Events
{
	ObjectHitHandler::ObjectHitHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent)
		: EventHandler(a_functionRef, a_args, a_isPersistent),
		  _sourceRefFilter(a_args, "kIncludedSourceRefs"sv, "kExcludedSourceRefs"sv),
		  _targetRefFilter(a_args, "kIncludedTargetRefs"sv, "kExcludedTargetRefs"sv),
		  _sourceObjectFilter(a_args, "kIncludedSourceObjects"sv, "kExcludedSourceObjects"sv),
		  _sourceProjectileFilter(a_args, "kIncludedSourceProjectiles"sv, "kExcludedSourceProjectiles"sv),
		  _materialNameFilter(a_args, "kIncludedMaterialNames"sv, "kExcludedMaterialNames"sv),
		  _flagsFilter(a_args, "iIncludedFlags"sv, "iExcludedFlags"sv)
	{
		RegisterEvents();
	}

	ObjectHitHandler::~ObjectHitHandler() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl ObjectHitHandler::ProcessEvent(const RE::TESHitEvent& a_event,
		RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
	{
		if (Script::ScriptFunctionRef::IsVMFrozen()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (!a_eventSource) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* targetRef = a_event.targetRef.get();
		if (!targetRef) [[unlikely]] {
			REX::Assert(false);
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto& eventHitData = a_event.hitData;

		const auto* sourceRef = a_event.causeRef.get();
		const auto* sourceObject = RE::TESForm::FindFormByNumericID(a_event.sourceFormID);
		const auto* sourceProjectileRef = eventHitData.sourceRef.underlying();
		const auto* sourceProjectile = RE::TESForm::FindFormByNumericID<RE::BGSProjectile>(a_event.projectileFormID);
		const auto& materialName = a_event.material;

		if (!_sourceRefFilter.IsMatch(sourceRef) ||
			!_targetRefFilter.IsMatch(targetRef) ||
			!_sourceObjectFilter.IsMatch(sourceObject) ||
			!(sourceProjectileRef ? _sourceProjectileFilter.IsMatch(sourceProjectileRef) : _sourceProjectileFilter.IsMatch(sourceProjectile)) ||
			!_materialNameFilter.IsMatch(materialName) ||
			!_flagsFilter.IsMatch(eventHitData.flags.get())) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto params = EventParams::Create();
		params.Insert("kSourceRef"sv, sourceRef);
		params.Insert("kTargetRef"sv, targetRef);
		params.Insert("kSourceObject"sv, sourceObject);
		params.Insert("kSourceProjectileRef"sv, sourceProjectileRef);
		params.Insert("kSourceProjectile"sv, sourceProjectile);
		params.Insert("sMaterialName"sv, materialName);

		auto hitData = HitData();

		if (a_event.usesHitData) {
			const auto* materialType = RE::BGSMaterialType::GetMaterialTypeByID(eventHitData.materialID);

			hitData = HitData::Create();
			hitData.Insert("iFlags"sv, eventHitData.flags.underlying());
			hitData.Insert("kHitEffect"sv, eventHitData.hitEffect);
			hitData.Insert("kCriticalEffect"sv, eventHitData.criticalEffect);
			hitData.Insert("kAmmo"sv, eventHitData.ammo);
			hitData.Insert("kMaterialType"sv, materialType);
			hitData.Insert("fBaseDamage"sv, eventHitData.healthDamage);
			hitData.Insert("fTotalDamage"sv, eventHitData.totalDamage);
			hitData.Insert("fPhysicalDamage"sv, eventHitData.physicalDamage);
			hitData.Insert("fLimbDamage"sv, eventHitData.targetedLimbDamage);
			hitData.Insert("fBlockedDamageMult"sv, eventHitData.percentBlocked);
			hitData.Insert("fResistedPhysicalDamage"sv, eventHitData.resistedPhysicalDamage);
			hitData.Insert("fResistedTypedDamage"sv, eventHitData.resistedTypedDamage);
			hitData.Insert("fReflectedDamage"sv, eventHitData.reflectedDamage);
			hitData.Insert("fSneakAttackMult"sv, eventHitData.sneakAttackBonus);
			hitData.Insert("fCriticalDamageMult"sv, eventHitData.criticalDamageMult);
			hitData.Insert("fBonusHealthDamageMult"sv, eventHitData.bonusHealthDamageMult);
			hitData.Insert("fPushBackMult"sv, eventHitData.pushBack);
			hitData.Insert("iStaggerMagnitude"sv, eventHitData.staggerMagnitude.underlying());
			hitData.Insert("iLimbLocation"sv, eventHitData.damageLimb.underlying());
		}

		params.Insert("kHitData"sv, std::move(hitData));

		_functionRef.InvokeFunction(std::move(params));
		return RE::BSEventNotifyControl::kContinue;
	}

	void ObjectHitHandler::RegisterEvents()
	{
		auto* eventSource = RE::TESHitEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void ObjectHitHandler::UnregisterEvents()
	{
		auto* eventSource = RE::TESHitEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
