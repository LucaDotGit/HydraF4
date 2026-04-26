#include "Plugin/Papyrus/Forms/WorldSpace.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::WorldSpace
{
	using WorldSpaceFlags = RE::TESWorldSpace::Flags;
	using WorldSpaceRecordFlags = RE::TESWorldSpace::RecordFlags;

	static RE::BGSLocation* GetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_worldSpace->location).load(std::memory_order_acquire);
	}

	static void SetLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::BGSLocation* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_worldSpace->location).store(a_value, std::memory_order_release);
	}

	static RE::BGSEncounterZone* GetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_worldSpace->encounterZone).load(std::memory_order_acquire);
	}

	static void SetEncounterZone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::BGSEncounterZone* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_worldSpace->encounterZone).store(a_value, std::memory_order_release);
	}

	static RE::BGSLightingTemplate* GetInteriorLighting(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_worldSpace->lightingTemplate).load(std::memory_order_acquire);
	}

	static void SetInteriorLighting(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::BGSLightingTemplate* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_worldSpace->lightingTemplate).store(a_value, std::memory_order_release);
	}

	static RE::TESClimate* GetClimate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return a_worldSpace->GetClimate();
	}

	static void SetClimate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::TESClimate* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->SetClimate(a_value);
	}

	static RE::BGSMusicType* GetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_worldSpace->musicType).load(std::memory_order_acquire);
	}

	static void SetMusicType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::BGSMusicType* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_worldSpace->musicType).store(a_value, std::memory_order_release);
	}

	static RE::TESWaterForm* GetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return a_worldSpace->GetWaterType();
	}

	static void SetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::TESWaterForm* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->SetWaterType(a_value);
	}

	static RE::TESWaterForm* GetLodWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return a_worldSpace->GetLODWaterType();
	}

	static void SetLodWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::TESWaterForm* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->SetLODWaterType(a_value);
	}

	static REX::Float32 GetLodWaterHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_worldSpace->GetLODWaterHeight();
	}

	static void SetLodWaterHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		REX::Float32 a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->SetLODWaterHeight(a_value);
	}

	static RE::TESObjectCELL* GetSkyCell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return nullptr;
		}

		return a_worldSpace->GetSkyCell();
	}

	static void SetSkyCell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::TESObjectCELL* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->SetSkyCell(a_value);
	}

	static bool ContainsLoadedCell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::TESObjectCELL* a_cell)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return false;
		}

		if (!a_cell) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CELL_NULL, a_stackId);
			return false;
		}

		const auto cellXY = a_cell->GetCellXAndY();
		if (!cellXY.has_value()) {
			return false;
		}

		const auto&& [cellX, cellY] = std::move(cellXY).value();
		return a_worldSpace->GetCellFromCellCoord(cellX, cellY) == a_cell;
	}

	static std::optional<std::vector<RE::TESObjectCELL*>> GetLoadedCells(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& cellMap = a_worldSpace->cellMap;

		auto result = std::vector<RE::TESObjectCELL*>();
		result.reserve(cellMap.size());

		for (const auto& [_, cell] : cellMap) {
			result.push_back(cell);
		}

		return result;
	}

	static bool ContainsLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		RE::BGSLocation* a_value)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return false;
		}

		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LOCATION_NULL, a_stackId);
			return false;
		}

		return a_worldSpace->locationMap.contains(a_value->GetFormID());
	}

	static std::optional<std::vector<RE::BGSLocation*>> GetLocations(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& locationMap = a_worldSpace->locationMap;

		auto result = std::vector<RE::BGSLocation*>();
		result.reserve(locationMap.size());

		for (const auto& [_, location] : locationMap) {
			result.push_back(location);
		}

		return result;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		WorldSpaceFlags a_flag)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return false;
		}

		return a_worldSpace->flags.any_atomic(a_flag);
	}

	static WorldSpaceFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return WorldSpaceFlags::kNone;
		}

		return a_worldSpace->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		WorldSpaceFlags a_flags)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESWorldSpace* a_worldSpace,
		WorldSpaceFlags a_flag,
		bool a_set)
	{
		if (!a_worldSpace) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::WORLD_SPACE_NULL, a_stackId);
			return;
		}

		a_worldSpace->flags.set_atomic(a_set, a_flag);
	}

	static WorldSpaceFlags GetFlag_IsSmallWorld(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kSmallWorld; }
	static WorldSpaceFlags GetFlag_DisallowFastTravel(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kCannotFastTravel; }
	static WorldSpaceFlags GetFlag_HasNoLodWater(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kNoLODWater; }
	static WorldSpaceFlags GetFlag_HasNoLandscape(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kNoLandscape; }
	static WorldSpaceFlags GetFlag_HasNoSky(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kNoSky; }
	static WorldSpaceFlags GetFlag_HasFixedDimensions(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kFixedDimensions; }
	static WorldSpaceFlags GetFlag_HasNoGrass(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceFlags::kNoGrass; }

	static WorldSpaceRecordFlags GetRecordFlag_DisallowWaiting(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return WorldSpaceRecordFlags::kCannotWait; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLocation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLocation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEncounterZone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEncounterZone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInteriorLighting);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetInteriorLighting);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetClimate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetClimate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMusicType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMusicType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLodWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLodWaterType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLodWaterHeight);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLodWaterHeight);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSkyCell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSkyCell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsLoadedCell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLoadedCells);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsLocation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLocations);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSmallWorld);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisallowFastTravel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoLodWater);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoLandscape);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoSky);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasFixedDimensions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoGrass);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_DisallowWaiting);
	}
}
