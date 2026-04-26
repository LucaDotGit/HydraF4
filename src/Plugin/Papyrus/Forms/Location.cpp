#include "Plugin/Papyrus/Forms/Location.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Location
{
	using LocationChangeFlags = RE::BGSLocation::ChangeFlags;

	static bool IsChildOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_childLocation,
		RE::BGSLocation* a_parentLocation)
	{
		if (!a_childLocation || !a_parentLocation) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		return a_childLocation->IsChildOf(a_parentLocation);
	}

	static bool IsParentOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_parentLocation,
		RE::BGSLocation* a_childLocation)
	{
		if (!a_parentLocation || !a_childLocation) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		return a_parentLocation->IsParentOf(a_childLocation);
	}

	static RE::BGSLocation* GetParent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_location->parentLoc).load(std::memory_order_acquire);
	}

	static void SetParent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		RE::BGSLocation* a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->parentLoc).store(a_value, std::memory_order_release);
	}

	static RE::BGSMusicType* GetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_location->musicType).load(std::memory_order_acquire);
	}

	static void SetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		RE::BGSMusicType* a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->musicType).store(a_value, std::memory_order_release);
	}

	static RE::BGSEncounterZone* GetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_location->zone).load(std::memory_order_acquire);
	}

	static void SetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		RE::BGSEncounterZone* a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->zone).store(a_value, std::memory_order_release);
	}

	static RE::ObjectRefHandle GetMapMarkerRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		return a_location->worldLocMarker;
	}

	static void SetMapMarkerRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		RE::ObjectRefHandle a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		a_location->worldLocMarker = a_value;
	}

	static REX::Float32 GetWorldRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_location->worldLocRadius).load(std::memory_order_acquire);
	}

	static void SetWorldRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		REX::Float32 a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->worldLocRadius).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetActorFadeMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_location->actorFadeMult).load(std::memory_order_acquire);
	}

	static void SetActorFadeMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		REX::Float32 a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->actorFadeMult).store(a_value, std::memory_order_release);
	}

	static bool GetIsCleared(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_location->cleared).load(std::memory_order_acquire);
	}

	static void SetIsCleared(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		bool a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->cleared).store(a_value, std::memory_order_release);
		a_location->AddChange(LocationChangeFlags::kCleared);

		if (a_value) {
			std::atomic_ref(a_location->everCleared).store(true, std::memory_order_release);
		}
	}

	static bool GetHasEverBeenCleared(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_location->everCleared).load(std::memory_order_acquire);
	}

	static void SetHasEverBeenCleared(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		bool a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_location->everCleared).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<RE::BGSLocation*>> GetChildLocations(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_parentLocation)
	{
		if (!a_parentLocation) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto result = std::vector<RE::BGSLocation*>();
		for (auto* child : dataHandler->GetFormArray<RE::BGSLocation>()) {
			if (child && child != a_parentLocation && child->parentLoc == a_parentLocation) {
				result.push_back(child);
			}
		}

		return result;
	}

	static std::uint32_t GetLocationLinkCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return 0;
		}

		const auto& linkedLocations = RE::BGSLocation::GetLinkedLocations();

		const auto locationIt = linkedLocations.find(a_location->GetFormID());
		if (locationIt == linkedLocations.end()) {
			return 0;
		}

		const auto* linkList = locationIt->second;
		if (!linkList) {
			return 0;
		}

		return linkList->size();
	}

	static RE::BGSLocation* GetNthLocationLinkLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		std::uint32_t a_index)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		const auto& linkedLocations = RE::BGSLocation::GetLinkedLocations();

		const auto locationIt = linkedLocations.find(a_location->GetFormID());
		if (locationIt == linkedLocations.end()) {
			return nullptr;
		}

		const auto* linkList = locationIt->second;
		if (!linkList || a_index >= linkList->size()) {
			return nullptr;
		}

		const auto& element = (*linkList)[a_index];
		return RE::TESForm::FindFormByNumericID<RE::BGSLocation>(element.first);
	}

	static RE::BGSKeyword* GetNthLocationLinkKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		std::uint32_t a_index)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		const auto& linkedLocations = RE::BGSLocation::GetLinkedLocations();

		const auto locationIt = linkedLocations.find(a_location->GetFormID());
		if (locationIt == linkedLocations.end()) {
			return nullptr;
		}

		const auto* linkList = locationIt->second;
		if (!linkList || a_index >= linkList->size()) {
			return nullptr;
		}

		const auto& element = (*linkList)[a_index];
		return RE::TESForm::FindFormByNumericID<RE::BGSKeyword>(element.second);
	}

	static std::uint32_t GetKeywordDataCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return 0;
		}

		const auto locationLock = RE::BSAutoReadLock(a_location->lock);
		return a_location->keywordDataArray.size();
	}

	static RE::BGSKeyword* GetNthKeywordDataKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		std::uint32_t a_index)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return nullptr;
		}

		const auto locationLock = RE::BSAutoReadLock(a_location->lock);

		const auto& keywordDataArray = a_location->keywordDataArray;
		if (a_index >= keywordDataArray.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, keywordDataArray.size());
			return nullptr;
		}

		return keywordDataArray[a_index].keyword;
	}

	static REX::Float32 GetNthKeywordDataValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		std::uint32_t a_index)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto locationLock = RE::BSAutoReadLock(a_location->lock);

		const auto& keywordDataArray = a_location->keywordDataArray;
		if (a_index >= keywordDataArray.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, keywordDataArray.size());
			return 0.0_f32;
		}

		return keywordDataArray[a_index].data;
	}

	static bool SetNthKeywordDataValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		std::uint32_t a_index,
		REX::Float32 a_value)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		const auto locationLock = RE::BSAutoWriteLock(a_location->lock);

		auto& keywordDataArray = a_location->keywordDataArray;
		if (a_index >= keywordDataArray.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, keywordDataArray.size());
			return false;
		}

		auto& entry = keywordDataArray[a_index];
		entry.data = a_value;

		a_location->AddChange(LocationChangeFlags::kKeywordData);
		return true;
	}

	static std::uint32_t GetKeywordDataIndexByKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSLocation* a_location,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_location) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return std::numeric_limits<std::uint32_t>::max();
		}

		if (!a_keyword) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::KEYWORD_NULL, a_stackId);
			return std::numeric_limits<std::uint32_t>::max();
		}

		const auto locationLock = RE::BSAutoReadLock(a_location->lock);

		const auto& keywordDataArray = a_location->keywordDataArray;
		for (auto i = 0ui32; i < keywordDataArray.size(); i++) {
			if (keywordDataArray[i].keyword == a_keyword) {
				return static_cast<std::int32_t>(i);
			}
		}

		return std::numeric_limits<std::uint32_t>::max();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsChildOf);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsParentOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetParent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMusicType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMusicType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEncounterZone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEncounterZone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMapMarkerRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMapMarkerRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWorldRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWorldRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActorFadeMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetActorFadeMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsCleared);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsCleared);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHasEverBeenCleared);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHasEverBeenCleared);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetChildLocations);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLocationLinkCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthLocationLinkLocation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthLocationLinkKeyword);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeywordDataCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNthKeywordDataKeyword);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNthKeywordDataValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetNthKeywordDataValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeywordDataIndexByKeyword);
	}
}
