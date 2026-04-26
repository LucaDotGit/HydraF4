#include "Plugin/Papyrus/Forms/EncounterZone.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::EncounterZone
{
	using EncounterZoneFlags = RE::ENCOUNTER_ZONE_DATA::Flags;
	using EncounterZoneChangeFlags = RE::BGSEncounterZone::ChangeFlags;

	static RE::TESForm* GetOwner(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_encounterZone->data.zoneOwner).load(std::memory_order_acquire);
	}

	static void SetOwner(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		RE::TESForm* a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->data.zoneOwner).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetOwnerRank(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->data.ownerRank).load(std::memory_order_acquire);
	}

	static void SetOwnerRank(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint8_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->data.ownerRank).store(a_value, std::memory_order_release);
	}

	static RE::BGSLocation* GetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_encounterZone->data.location).load(std::memory_order_acquire);
	}

	static void SetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		RE::BGSLocation* a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->data.location).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetMinLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->data.minLevel).load(std::memory_order_acquire);
	}

	static void SetMinLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint8_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->data.minLevel).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetMaxLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->data.maxLevel).load(std::memory_order_acquire);
	}

	static void SetMaxLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint8_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->data.maxLevel).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_zone)
	{
		if (!a_zone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_zone->gameData.zoneLevel).load(std::memory_order_acquire);
	}

	static void SetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_zone,
		std::uint16_t a_value)
	{
		if (!a_zone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_zone->gameData.zoneLevel).store(a_value, std::memory_order_release);
		a_zone->AddChange(EncounterZoneChangeFlags::kGameData);
	}

	static std::uint32_t GetDetachTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->gameData.detachTime).load(std::memory_order_acquire);
	}

	static void SetDetachTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint32_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->gameData.detachTime).store(a_value, std::memory_order_release);
		a_encounterZone->AddChange(EncounterZoneChangeFlags::kGameData);
	}

	static std::uint32_t GetAttachTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->gameData.attachTime).load(std::memory_order_acquire);
	}

	static void SetAttachTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint32_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->gameData.attachTime).store(a_value, std::memory_order_release);
		a_encounterZone->AddChange(EncounterZoneChangeFlags::kGameData);
	}

	static std::uint32_t GetResetTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_encounterZone->gameData.resetTime).load(std::memory_order_acquire);
	}

	static void SetResetTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		std::uint32_t a_value)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_encounterZone->gameData.resetTime).store(a_value, std::memory_order_release);
		a_encounterZone->AddChange(EncounterZoneChangeFlags::kGameData);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		EncounterZoneFlags a_flag)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return false;
		}

		return a_encounterZone->data.flags.any_atomic(a_flag);
	}

	static EncounterZoneFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return EncounterZoneFlags::kNone;
		}

		return a_encounterZone->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		EncounterZoneFlags a_flags)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		a_encounterZone->data.flags.store_atomic(a_flags);
		a_encounterZone->AddChange(EncounterZoneChangeFlags::kFlags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSEncounterZone* a_encounterZone,
		EncounterZoneFlags a_flag,
		bool a_set)
	{
		if (!a_encounterZone) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ENCOUNTER_ZONE_NULL, a_stackId);
			return;
		}

		a_encounterZone->data.flags.set_atomic(a_set, a_flag);
		a_encounterZone->AddChange(EncounterZoneChangeFlags::kFlags);
	}

	static EncounterZoneFlags GetFlag_NeverReset(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EncounterZoneFlags::kNeverReset; }
	static EncounterZoneFlags GetFlag_MatchPlayerBelowMin(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EncounterZoneFlags::kMatchPCBelowMin; }
	static EncounterZoneFlags GetFlag_DisableCombatBoundary(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EncounterZoneFlags::kDisableCombatBoundary; }
	static EncounterZoneFlags GetFlag_IsWorkshopZone(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return EncounterZoneFlags::kWorkshopZone; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOwner);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOwner);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOwnerRank);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOwnerRank);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLocation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLocation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMinLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMinLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaxLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaxLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDetachTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDetachTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAttachTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAttachTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetResetTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetResetTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_NeverReset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_MatchPlayerBelowMin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisableCombatBoundary);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsWorkshopZone);
	}
}