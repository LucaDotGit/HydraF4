#include "Plugin/Papyrus/Forms/Cell.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::Cell
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	using CellFlags = RE::TESObjectCELL::Flags;
	using CellRecordFlags = RE::TESObjectCELL::RecordFlags;

	static RE::TESForm* GetOwner(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraOwnership>();
		return extraData ? extraData->owner : nullptr;
	}

	static void SetOwner(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::TESForm* a_value)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraOwnership>();
		extraData->owner = a_value;

		a_cell->AddChange(RE::TESObjectCELL::ChangeFlags::kOwnership);
	}

	static RE::BGSLocation* GetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraLocation>();
		return extraData ? extraData->location : nullptr;
	}

	static void SetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSLocation* a_value)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraLocation>();
		extraData->location = a_value;
	}

	static RE::BGSEncounterZone* GetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraEncounterZone>();
		return extraData ? extraData->encounterZone : nullptr;
	}

	static void SetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSEncounterZone* a_value)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraEncounterZone>();
		extraData->encounterZone = a_value;
	}

	static RE::BGSAcousticSpace* GetAcousticSpace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellAcousticSpace>();
		return extraData ? extraData->acousticSpace : nullptr;
	}

	static void SetAcousticSpace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSAcousticSpace* a_acousticSpace)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCellAcousticSpace>();
		extraData->acousticSpace = a_acousticSpace;
	}

	static RE::BGSLightingTemplate* GetLightingTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_cell->lightingTemplate).load(std::memory_order_acquire);
	}

	static void SetLightingTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSLightingTemplate* a_value)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_cell->lightingTemplate).store(a_value, std::memory_order_release);
	}

	static RE::BGSGodRays* GetGodRays(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellGodRays>();
		return extraData ? extraData->godRays : nullptr;
	}

	static void SetGodRays(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSGodRays* a_godRays)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCellGodRays>();
		extraData->godRays = a_godRays;
	}

	static RE::TESImageSpace* GetImageSpace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellImageSpace>();
		return extraData ? extraData->imageSpace : nullptr;
	}

	static void SetImageSpace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::TESImageSpace* a_imageSpace)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCellImageSpace>();
		extraData->imageSpace = a_imageSpace;
	}

	static RE::BGSMusicType* GetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellMusicType>();
		return extraData ? extraData->musicType : nullptr;
	}

	static void SetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::BGSMusicType* a_music)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetExtra<RE::ExtraCellMusicType>();
		extraData->musicType = a_music;
	}

	static RE::TESWaterForm* GetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellWaterType>();
		return extraData ? extraData->waterType : nullptr;
	}

	static void SetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::TESWaterForm* a_water)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCellWaterType>();
		extraData->waterType = a_water;
	}

	static RE::TESRegion* GetSkyRegion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);
		const auto* extraData = extraList->GetExtra<RE::ExtraCellSkyRegion>();
		return extraData ? extraData->region : nullptr;
	}

	static void SetSkyRegion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		RE::TESRegion* a_region)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCellSkyRegion>();
		extraData->region = a_region;
	}

	static std::optional<std::vector<RE::TESRegion*>> GetRegions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return std::nullopt;
		}

		const auto extraList = a_cell->extraList;
		if (!extraList) {
			return std::nullopt;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraRegionList>();
		if (!extraData) {
			return std::nullopt;
		}

		const auto* regionList = extraData->regionList;
		if (!regionList) {
			return std::nullopt;
		}

		auto result = std::vector<RE::TESRegion*>();
		for (auto* region : *regionList) {
			if (region) {
				result.push_back(region);
			}
		}

		return result;
	}

	static void SetRegions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		std::optional<std::vector<RE::TESRegion*>> a_values)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		const auto extraList = a_cell->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraRegionList>();
		auto& extraRegionList = reinterpret_cast<std::unique_ptr<RE::TESRegionList>&>(extraData->regionList);

		if (!a_values.has_value()) {
			extraRegionList.reset();
			return;
		}

		if (!extraRegionList) {
			extraRegionList = std::make_unique<RE::TESRegionList>();
		}

		extraRegionList->SetRegions(a_values.value());
	}

	static std::optional<std::vector<RE::NiPointer<RE::TESObjectREFR>>> FindAllRefs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_originCell,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		if (!a_originCell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return std::nullopt;
		}

		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		a_originCell->ForEachRef([&scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
			if (scriptFilter.IsMatch(a_ref.get())) {
				result.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static std::optional<std::vector<RE::NiPointer<RE::TESObjectREFR>>> FindAllRefsInRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_originCell,
		std::optional<Vector3Struct> a_center,
		REX::Float32 a_radius,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		if (!a_originCell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_center.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		if (a_radius <= 0.0_f32) {
			return std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		}

		const auto center = Vector3Struct::ToNiPoint3(a_center.value());
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		a_originCell->ForEachRefInRange(center, a_radius, [&scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
			if (scriptFilter.IsMatch(a_ref.get())) {
				result.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static std::optional<std::vector<RE::NiPointer<RE::TESObjectREFR>>> FindAllRefsInRangeFromRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_originRef,
		REX::Float32 a_radius,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		if (!a_originRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		if (a_radius <= 0.0_f32) {
			return std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		}

		const auto* parentCell = a_originRef->parentCell;
		const auto center = a_originRef->data.location;
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		parentCell->ForEachRefInRange(center, a_radius, [a_originRef, &scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
			if (a_ref.get() != a_originRef && scriptFilter.IsMatch(a_ref.get())) {
				result.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		CellFlags a_flag)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return false;
		}

		return a_cell->cellFlags.any_atomic(a_flag);
	}

	static CellFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return CellFlags::kNone;
		}

		return a_cell->cellFlags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		CellFlags a_flags)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		a_cell->cellFlags.store_atomic(a_flags);
		a_cell->AddChange(RE::TESObjectCELL::ChangeFlags::kFlags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCELL* a_cell,
		CellFlags a_flag,
		bool a_set)
	{
		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return;
		}

		a_cell->cellFlags.set_atomic(a_set, a_flag);
		a_cell->AddChange(RE::TESObjectCELL::ChangeFlags::kFlags);
	}

	static CellFlags GetFlag_IsInterior(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kIsInterior; }
	static CellFlags GetFlag_HasWater(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kHasWater; }
	static CellFlags GetFlag_AllowFastTravel(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kCanTravelFromHere; }
	static CellFlags GetFlag_HasNoLodWater(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kNoLODWater; }
	static CellFlags GetFlag_HasTempData(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kHasTempData; }
	static CellFlags GetFlag_IsPublicArea(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kPublicArea; }
	static CellFlags GetFlag_IsHandChanged(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kHandChanged; }
	static CellFlags GetFlag_ShowSky(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kShowSky; }
	static CellFlags GetFlag_UseSkyLighting(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kUseSkyLighting; }
	static CellFlags GetFlag_HasExitWarning(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kWarnToLeave; }
	static CellFlags GetFlag_IsHiddenFromInteriorList(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kHiddenFromInteriorList; }
	static CellFlags GetFlag_UseSunlightShadows(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kSunlightShadows; }
	static CellFlags GetFlag_HasDistantLodOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kDistantLODOnly; }
	static CellFlags GetFlag_DisallowCompanionFastTravel(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellFlags::kFollowersCannotTravelHere; }

	static CellRecordFlags GetRecordFlag_HasNoPreVis(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellRecordFlags::kNoPreVis; }
	static CellRecordFlags GetRecordFlag_IsPersistent(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellRecordFlags::kPersistent; }
	static CellRecordFlags GetRecordFlag_IsOffLimits(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellRecordFlags::kOffLimits; }
	static CellRecordFlags GetRecordFlag_IsCompressed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellRecordFlags::kCompressed; }
	static CellRecordFlags GetRecordFlag_DisallowWaiting(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CellRecordFlags::kCannotWait; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetOwner);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetOwner);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLocation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLocation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEncounterZone);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetEncounterZone);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAcousticSpace);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAcousticSpace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLightingTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLightingTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGodRays);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetGodRays);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetImageSpace);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetImageSpace);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetMusicType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetMusicType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSkyRegion);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSkyRegion);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRegions);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRegions);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefsInRange);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefsInRangeFromRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsInterior);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasWater);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowFastTravel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoLodWater);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasTempData);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPublicArea);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHandChanged);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_ShowSky);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseSkyLighting);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasExitWarning);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHiddenFromInteriorList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseSunlightShadows);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasDistantLodOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisallowCompanionFastTravel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNoPreVis);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsPersistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsOffLimits);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsCompressed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_DisallowWaiting);
	}
}
