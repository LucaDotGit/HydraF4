#include "Plugin/Papyrus/Forms/ObjectReference.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::ObjectReference::Extra::Impl
{
	static constexpr auto TYPE_KEY = "iType"sv;
	static constexpr auto NAME_KEY = "sName"sv;

	static constexpr auto LINKED_DOOR_KEY = "kLinkedDoor"sv;
	static constexpr auto POSITION_KEY = "kPosition"sv;
	static constexpr auto ROTATION_KEY = "kRotation"sv;

	static constexpr auto OWNER_QUEST_KEY = "kOwnerQuest"sv;
	static constexpr auto LINKED_ALIAS_KEY = "kLinkedAlias"sv;
	static constexpr auto PACKAGES_KEY = "kPackages"sv;
}

namespace Plugin::Papyrus::Forms::ObjectReference::Extra
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	using RefChangeFlags = RE::TESObjectREFR::ChangeFlags;

	static MapMarkerData GetMapMarkerData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_markerRef)
	{
		if (!a_markerRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_markerRef->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraMarker = extraList->GetExtra<RE::ExtraMapMarker>();
		const auto* extraMarkerData = extraMarker ? extraMarker->mapMarkerData : nullptr;

		if (!extraMarkerData) {
			return nullptr;
		}

		auto markerData = MapMarkerData::Create();
		markerData.Insert(Impl::NAME_KEY, extraMarkerData->locationName.fullName);
		markerData.Insert(Impl::TYPE_KEY, extraMarkerData->type.underlying());

		return markerData;
	}

	static bool SetMapMarkerData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_markerRef,
		MapMarkerData a_data)
	{
		if (!a_markerRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!a_data) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto extraList = a_markerRef->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraMarker = extraList->GetOrCreateExtra<RE::ExtraMapMarker>();
		auto& extraMarkerData = reinterpret_cast<std::unique_ptr<RE::MapMarkerData>&>(extraMarker->mapMarkerData);

		if (!extraMarkerData) {
			extraMarkerData = std::make_unique<RE::MapMarkerData>();
		}

		extraMarkerData->type = a_data.Find<RE::MARKER_TYPE>(Impl::TYPE_KEY).value();
		extraMarkerData->locationName.fullName = a_data.Find<RE::BGSLocalizedString>(Impl::NAME_KEY).value();

		return true;
	}

	static DoorDestinationData GetDoorDestination(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_doorRef)
	{
		if (!a_doorRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_doorRef->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraTeleport = extraList->GetExtra<RE::ExtraTeleport>();
		if (!extraTeleport) {
			return nullptr;
		}

		const auto* extraTeleportData = extraTeleport->teleportData;
		if (!extraTeleportData) {
			return nullptr;
		}

		auto position = Vector3Struct::FromNiPoint3(extraTeleportData->position);
		auto rotation = Vector3Struct::FromNiPoint3(extraTeleportData->rotation);

		auto destinationData = DoorDestinationData::Create();
		destinationData.Insert(Impl::LINKED_DOOR_KEY, extraTeleportData->linkedDoor);
		destinationData.Insert(Impl::POSITION_KEY, position);
		destinationData.Insert(Impl::ROTATION_KEY, rotation);

		return destinationData;
	}

	static bool SetDoorDestination(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_doorRef,
		DoorDestinationData a_data)
	{
		if (!a_doorRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!a_data) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* linkedDoor = a_data.Find<RE::TESObjectREFR*>(Impl::LINKED_DOOR_KEY).value();
		if (!a_data) [[unlikely]] {
			a_vm.PostError("The specified linked door cannot be none"sv, a_stackId);
			return false;
		}

		const auto* baseObject = a_doorRef->GetBaseObject();
		if (!baseObject || !RE::DynamicCast<const RE::TESObjectDOOR*>(baseObject)) [[unlikely]] {
			a_vm.PostError("The specified linked door is not a door"sv, a_stackId);
			return false;
		}

		if (baseObject->formFlags.none_atomic(RE::TESObjectREFR::RecordFlags::kPersistent)) [[unlikely]] {
			a_vm.PostError("The specified linked door is not persistent"sv, a_stackId);
			return false;
		}

		const auto positionData = a_data.Find<std::optional<Vector3Struct>>(Impl::POSITION_KEY).value();
		const auto rotationData = a_data.Find<std::optional<Vector3Struct>>(Impl::ROTATION_KEY).value();

		const auto extraList = linkedDoor->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraTeleport = extraList->GetOrCreateExtra<RE::ExtraTeleport>();
		auto& extraTeleportData = reinterpret_cast<std::unique_ptr<RE::DoorTeleportData>&>(extraTeleport->teleportData);

		if (!extraTeleportData) {
			extraTeleportData = std::make_unique<RE::DoorTeleportData>();
		}

		constexpr auto DEFAULT_VECTOR = Vector3Struct{ .x = 999.0_f32, .y = 999.0_f32, .z = 999.0_f32 };

		auto position = DEFAULT_VECTOR;
		auto rotation = DEFAULT_VECTOR;

		if (positionData.has_value()) {
			position = Vector3Struct(positionData.value());
		}

		if (rotationData.has_value()) {
			rotation = Vector3Struct(rotationData.value());
		}

		extraTeleportData->transitionCell = linkedDoor->parentCell;
		extraTeleportData->linkedDoor = RE::ObjectRefHandle(linkedDoor);
		extraTeleportData->position = Vector3Struct::ToNiPoint3(position);
		extraTeleportData->rotation = Vector3Struct::ToNiPoint3(rotation);

		a_doorRef->AddChange(RefChangeFlags::kExtraTeleport);
		return true;
	}

	static std::optional<std::vector<AliasInstanceData>> GetAliasInstances(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto extraList = a_ref->extraList;
		if (!extraList) {
			return std::nullopt;
		}

		const auto* extraData = extraList->GetExtra<RE::ExtraAliasInstanceArray>();
		if (!extraData) {
			return std::nullopt;
		}

		const auto aliasLock = RE::BSAutoReadLock(extraData->aliasArrayLock);
		const auto& aliasArray = extraData->aliasArray;

		auto result = std::vector<AliasInstanceData>();
		result.reserve(aliasArray.size());

		for (const auto& aliasInstance : aliasArray) {
			const auto* packages = aliasInstance.instancedPackages;

			auto data = AliasInstanceData::Create();
			data.Insert(Impl::OWNER_QUEST_KEY, aliasInstance.quest);
			data.Insert(Impl::LINKED_ALIAS_KEY, aliasInstance.alias);
			data.Insert(Impl::PACKAGES_KEY, packages ? std::make_optional(*packages) : std::nullopt);

			result.push_back(std::move(data));
		}

		return result;
	}

	static std::optional<std::vector<AliasInstanceData>> GetAliasInstancesByQuest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESQuest* a_quest)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		if (!a_quest) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::QUEST_NULL, a_stackId);
			return std::nullopt;
		}

		const auto extraList = a_ref->extraList;
		if (!extraList) {
			return std::nullopt;
		}

		const auto* extraData = extraList->GetExtra<RE::ExtraAliasInstanceArray>();
		if (!extraData) {
			return std::nullopt;
		}

		const auto aliasLock = RE::BSAutoReadLock(extraData->aliasArrayLock);
		const auto& aliasArray = extraData->aliasArray;

		auto result = std::vector<AliasInstanceData>();
		for (const auto& aliasInstance : aliasArray) {
			if (aliasInstance.quest != a_quest) {
				continue;
			}

			const auto* packages = aliasInstance.instancedPackages;

			auto data = AliasInstanceData::Create();
			data.Insert(Impl::OWNER_QUEST_KEY, aliasInstance.quest);
			data.Insert(Impl::LINKED_ALIAS_KEY, aliasInstance.alias);
			data.Insert(Impl::PACKAGES_KEY, packages ? std::make_optional(*packages) : std::nullopt);

			result.push_back(std::move(data));
		}

		return result;
	}

	static RE::BGSLocalizedString GetDisplayName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return {};
		}

		const auto displayName = a_ref->GetDisplayName();
		if (!displayName.has_value()) {
			return {};
		}

		return std::move(displayName).value();
	}

	static void SetDisplayName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BGSLocalizedString a_name)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		a_ref->SetDisplayName(a_name);
	}

	static RE::ObjectRefHandle GetAshPileSourceRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_itemRef->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraAshPileRef>();
		if (!extraData) {
			return nullptr;
		}

		return extraData->ashPileRef;
	}

	static void SetAshPileSourceRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef,
		RE::TESObjectREFR* a_sourceRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_itemRef->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraAshPileRef>();
		extraData->ashPileRef = a_sourceRef;
	}

	static RE::ObjectRefHandle GetDroppedItemSourceRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_itemRef->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraItemDropper>();
		if (!extraData) {
			return nullptr;
		}

		return extraData->dropperRef;
	}

	static void SetDroppedItemSourceRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef,
		RE::TESObjectREFR* a_sourceRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_itemRef->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraItemDropper>();
		extraData->dropperRef = a_sourceRef;
	}

	static std::optional<std::vector<RE::ObjectRefHandle>> GetDroppedItemRefs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto extraList = a_itemRef->extraList;
		if (!extraList) {
			return std::nullopt;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraDroppedItemList>();
		if (!extraData) {
			return std::nullopt;
		}

		const auto& droppedItemList = extraData->droppedItemList;

		auto result = std::vector<RE::ObjectRefHandle>();
		for (const auto& handle : droppedItemList) {
			if (handle) {
				result.push_back(handle);
			}
		}

		return result;
	}

	static void SetDroppedItemRefs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef,
		std::vector<RE::ObjectRefHandle> a_droppedItemRefs)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_itemRef->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraDroppedItemList>();
		extraData->droppedItemList = { a_droppedItemRefs.begin(), a_droppedItemRefs.end() };
	}

	static std::uint16_t GetItemStackCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0;
		}

		const auto extraList = a_itemRef->extraList;
		if (!extraList) {
			return 1;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraLockData = extraList->GetExtra<RE::ExtraCount>();
		if (!extraLockData) {
			return 1;
		}

		return extraLockData->count;
	}

	static void SetItemStackCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_itemRef,
		std::uint16_t a_value)
	{
		if (!a_itemRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_itemRef->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraCount>();
		extraData->count = a_value;
	}

	static std::uint32_t GetLockTryCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0;
		}

		const auto extraList = a_ref->extraList;
		if (!extraList) {
			return 0;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraLock>();
		if (!extraData) {
			return 0;
		}

		const auto* extraLockData = extraData->lock;
		if (!extraLockData) {
			return 0;
		}

		return extraLockData->numTries;
	}

	static void SetLockTryCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		std::uint32_t a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_ref->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraLock>();
		auto& extraLockData = reinterpret_cast<std::unique_ptr<RE::REFR_LOCK>&>(extraData->lock);

		if (!extraLockData) {
			extraLockData = std::make_unique<RE::REFR_LOCK>();
		}

		extraLockData->numTries = a_value;
		a_ref->AddChange(RefChangeFlags::kExtraLock);
	}

	static RE::TESKey* GetLockKey(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto extraList = a_ref->extraList;
		if (!extraList) {
			return nullptr;
		}

		const auto extraLock = RE::BSAutoReadLock(extraList->extraRWLock);

		const auto* extraData = extraList->GetExtra<RE::ExtraLock>();
		if (!extraData) {
			return nullptr;
		}

		const auto* extraLockData = extraData->lock;
		return extraLockData ? extraLockData->key : nullptr;
	}

	static void SetLockKey(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESKey* a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		const auto extraList = a_ref->GetOrCreateExtraList();
		const auto extraLock = RE::BSAutoWriteLock(extraList->extraRWLock);

		auto* extraData = extraList->GetOrCreateExtra<RE::ExtraLock>();
		auto& extraLockData = reinterpret_cast<std::unique_ptr<RE::REFR_LOCK>&>(extraData->lock);

		if (!extraLockData) {
			extraLockData = std::make_unique<RE::REFR_LOCK>();
		}

		extraLockData->key = a_value;
		a_ref->AddChange(RefChangeFlags::kExtraLock);
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetMapMarkerData);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetMapMarkerData);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDoorDestination);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDoorDestination);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAliasInstances);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAliasInstancesByQuest);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDisplayName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDisplayName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAshPileSourceRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAshPileSourceRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDroppedItemSourceRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDroppedItemSourceRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDroppedItemRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDroppedItemRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetItemStackCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetItemStackCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLockTryCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLockTryCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLockKey);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLockKey);
	}
}
