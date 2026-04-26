#include "Plugin/Papyrus/Game.hpp"

#include "Plugin/Internal/Events/Managers/CrosshairRefManager.hpp"
#include "Plugin/Internal/EventsManager.hpp"
#include "Plugin/Internal/Game/GameGameplaySettings.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/GameStartedManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"
#include "Plugin/Internal/Structs/Versions.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::Game::Impl
{
	[[nodiscard]] __forceinline static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsByProcessLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::PROCESS_TYPE a_level,
		std::span<RE::BSScript::SharedVariant> a_includedObjects,
		std::span<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		constexpr auto MIN_PROCESS_LEVEL = RE::PROCESS_TYPE::kMin;
		constexpr auto MAX_PROCESS_LEVEL = RE::PROCESS_TYPE::kMax;

		if (a_level < MIN_PROCESS_LEVEL || a_level > MAX_PROCESS_LEVEL) [[unlikely]] {
			a_vm.PostError("The specified process level {} is out of range ({} - {})"sv,
				a_stackId, std::to_underlying(a_level), std::to_underlying(MIN_PROCESS_LEVEL), std::to_underlying(MAX_PROCESS_LEVEL));
			return std::nullopt;
		}

		const auto* processLists = RE::ProcessLists::GetSingleton();
		if (!processLists) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::Actor>>();
		processLists->ForEachActorInProcess(a_level, [&scriptFilter, &result](const RE::NiPointer<RE::Actor>& a_actor) {
			if (scriptFilter.IsMatch(a_actor.get())) {
				result.push_back(a_actor);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static void OnPerkPointsChange(std::uint8_t a_newCount)
	{
		auto perkPointIncreaseEvent = RE::PerkPointIncreaseEvent{
			.newCount = a_newCount
		};

		F4SE::GetTaskInterface()->AddTask([perkPointIncreaseEvent]() {
			RE::PerkPointIncreaseEvent::GetEventSource()->Notify(perkPointIncreaseEvent);
		});
	}
}

namespace Plugin::Papyrus::Game
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;
	using VersionStruct = ::Plugin::Internal::Structs::Versions::VersionData;

	static VersionStruct GetVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return REL::Module::GetSingleton()->GetVersion();
	}

	static VersionStruct GetOldGenVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::RUNTIME_LATEST_OG.GetVersion();
	}

	static VersionStruct GetNextGenVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::RUNTIME_LATEST_NG.GetVersion();
	}

	static VersionStruct GetAnniversaryEditionVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::RUNTIME_LATEST_AE.GetVersion();
	}

	static bool GetIsOnlyOldGen(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeOnlyOG();
	}

	static bool GetIsOnlyNextGen(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeOnlyNG();
	}

	static bool GetIsOnlyAnniversaryEdition(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeOnlyAE();
	}

	static bool GetIsAtLeastOldGen(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeAtLeastOG();
	}

	static bool GetIsAtLeastNextGen(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeAtLeastNG();
	}

	static bool GetIsAtLeastAnniversaryEdition(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::IsRuntimeAtLeastAE();
	}

	static bool GetIsNewGame(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameStartedManager()->GetIsNewGame();
	}

	static bool GetIsFirstGameLoad(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameStartedManager()->GetIsFirstGameLoad();
	}

	static RE::DifficultyLevel GetDifficulty(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetGameplaySettings()->GetDifficulty();
	}

	static RE::DifficultyLevel GetDifficulty_VeryEasy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kVeryEasy; }
	static RE::DifficultyLevel GetDifficulty_Easy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kEasy; }
	static RE::DifficultyLevel GetDifficulty_Normal(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kNormal; }
	static RE::DifficultyLevel GetDifficulty_Hard(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kHard; }
	static RE::DifficultyLevel GetDifficulty_VeryHard(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kVeryHard; }
	static RE::DifficultyLevel GetDifficulty_FakeSurvival(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kSurvival; }
	static RE::DifficultyLevel GetDifficulty_Survival(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::DifficultyLevel::kTrueSurvival; }

	static std::uint32_t GetTeammateCount(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return std::atomic_ref(player->teammateCount).load(std::memory_order_acquire);
	}

	static RE::ObjectRefHandle GetCurrentCrosshairRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetEventsManager()->GetCrosshairRefManager()->GetCurrentRef();
	}

	static RE::TESObjectREFR* GetCurrentCommandTargetRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(player->commandTarget).load(std::memory_order_acquire);
	}

	static RE::ObjectRefHandle GetLastUsedPowerArmorRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return player->lastUsedPowerArmor;
	}

	static bool RemoveLastUsedPowerArmorRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto lastUsedPowerArmor = player->lastUsedPowerArmor;
		if (!lastUsedPowerArmor) {
			return false;
		}

		lastUsedPowerArmor.reset();

		F4SE::GetTaskInterface()->AddTask([player]() {
			player->RemoveLastUsedPowerArmor();
		});

		return true;
	}

	static std::optional<RE::BSTArray<RE::ObjectRefHandle>> GetAllMapMarkerRefs(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		return player->currentMapMarkers;
	}

	static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsByProcessLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::PROCESS_TYPE a_level,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		return Impl::GetActorsByProcessLevel(a_vm, a_stackId, a_level, a_includedObjects, a_excludedObjects);
	}

	static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsInHighProcess(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		return Impl::GetActorsByProcessLevel(a_vm, a_stackId, RE::PROCESS_TYPE::kHigh, a_includedObjects, a_excludedObjects);
	}

	static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsInMiddleHighProcess(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		return Impl::GetActorsByProcessLevel(a_vm, a_stackId, RE::PROCESS_TYPE::kMiddleHigh, a_includedObjects, a_excludedObjects);
	}

	static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsInMiddleLowProcess(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		return Impl::GetActorsByProcessLevel(a_vm, a_stackId, RE::PROCESS_TYPE::kMiddleLow, a_includedObjects, a_excludedObjects);
	}

	static std::optional<std::vector<RE::NiPointer<RE::Actor>>> GetActorsInLowProcess(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		return Impl::GetActorsByProcessLevel(a_vm, a_stackId, RE::PROCESS_TYPE::kLow, a_includedObjects, a_excludedObjects);
	}

	static std::optional<std::vector<RE::NiPointer<RE::TESObjectREFR>>> FindAllRefs(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		const auto* tes = RE::TES::GetSingleton();
		if (!tes) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		tes->ForEachRef([&scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
			if (scriptFilter.IsMatch(a_ref.get())) {
				result.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static std::optional<std::vector<RE::NiPointer<RE::TESObjectREFR>>> FindAllRefsInRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<Vector3Struct> a_center,
		REX::Float32 a_radius,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		if (!a_center.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		if (a_radius <= 0.0_f32) {
			return std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		}

		const auto* tes = RE::TES::GetSingleton();
		if (!tes) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto center = Vector3Struct::ToNiPoint3(a_center.value());
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		tes->ForEachRefInRange(center, a_radius, [&scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
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

		const auto* tes = RE::TES::GetSingleton();
		if (!tes) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto center = a_originRef->data.location;
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::NiPointer<RE::TESObjectREFR>>();
		tes->ForEachRefInRange(center, a_radius, [a_originRef, &scriptFilter, &result](const RE::NiPointer<RE::TESObjectREFR>& a_ref) {
			if (a_ref.get() != a_originRef && scriptFilter.IsMatch(a_ref.get())) {
				result.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static std::optional<std::vector<RE::TESObjectCELL*>> GetLoadedCells(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* tes = RE::TES::GetSingleton();
		if (!tes) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto* interior = tes->interiorCell;
		if (interior) {
			return std::vector<RE::TESObjectCELL*>{ interior };
		}

		const auto* grid = tes->gridCells;
		if (!grid) {
			return std::nullopt;
		}

		const auto gridLength = grid->length;

		auto result = std::vector<RE::TESObjectCELL*>();
		result.reserve(gridLength);

		for (auto i = 0ui32; i < gridLength; i++) {
			auto* cell = grid->cells[i];
			if (cell) {
				result.push_back(cell);
			}
		}

		return result;
	}

	static RE::TESWorldSpace* GetCurrentWorldSpace(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* tes = RE::TES::GetSingleton();
		if (!tes) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		auto* worldSpace = tes->worldSpace;
		if (worldSpace) {
			return worldSpace;
		}

		auto* interior = tes->interiorCell;
		if (!interior) {
			return nullptr;
		}

		return interior->worldSpace;
	}

	static RE::BGSNote* GetPlayingHolotape(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(player->currHolotape).load(std::memory_order_acquire);
	}

	static void PlayHolotape(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([player, a_holotape]() {
			player->PlayHolotape(a_holotape);
		});

		return;
	}

	static bool StopHolotape(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return false;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto* currentHolotape = player->currHolotape;
		if (currentHolotape != a_holotape) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([player, a_holotape]() {
			player->StopHolotape(a_holotape);
		});

		return true;
	}

	static bool GetIsPipBoyLightOn(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return player->IsPipboyLightOn();
	}

	static void SetIsPipBoyLightOn(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([player, a_value]() {
			constexpr auto SKIP_EFFECTS = false;
			player->ShowPipboyLight(a_value, SKIP_EFFECTS);
		});
	}

	static bool GetIsTimeFrozen(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* main = RE::Main::GetSingleton();
		if (!main) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return std::atomic_ref(main->freezeTime).load(std::memory_order_acquire);
	}

	static void SetIsTimeFrozen(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		auto* main = RE::Main::GetSingleton();
		if (!main) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		std::atomic_ref(main->freezeTime).store(a_value, std::memory_order_release);
	}

	static RE::TESRace* GetPlayerCharGenRace(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(player->charGenRace).load(std::memory_order_acquire);
	}

	static void SetPlayerCharGenRace(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESRace* a_value)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!a_value) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		std::atomic_ref(player->charGenRace).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetPerkPoints(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return std::atomic_ref(player->perkCount).load(std::memory_order_acquire);
	}

	static void SetPerkPoints(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_value)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		auto& perkCount = player->perkCount;

		const auto oldPerkCount = perkCount;
		const auto newPerkCount = static_cast<std::uint8_t>(std::clamp(a_value,
			static_cast<std::int32_t>(std::numeric_limits<std::uint8_t>::min()),
			static_cast<std::int32_t>(std::numeric_limits<std::uint8_t>::max())));

		if (oldPerkCount == newPerkCount) {
			return;
		}

		perkCount = newPerkCount;
		Impl::OnPerkPointsChange(newPerkCount);
	}

	static void ModPerkPoints(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_amount)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		auto& perkCount = player->perkCount;

		const auto oldPerkCount = perkCount;
		const auto newPerkCount = static_cast<std::uint8_t>(std::clamp(oldPerkCount + a_amount,
			static_cast<std::int32_t>(std::numeric_limits<std::uint8_t>::min()),
			static_cast<std::int32_t>(std::numeric_limits<std::uint8_t>::max())));

		if (oldPerkCount == newPerkCount) {
			return;
		}

		perkCount = newPerkCount;
		Impl::OnPerkPointsChange(newPerkCount);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOldGenVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNextGenVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAnniversaryEditionVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsOnlyOldGen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsOnlyNextGen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsOnlyAnniversaryEdition);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsAtLeastOldGen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsAtLeastNextGen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsAtLeastAnniversaryEdition);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsNewGame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsFirstGameLoad);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDifficulty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_VeryEasy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_Easy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_Normal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_Hard);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_VeryHard);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_FakeSurvival);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDifficulty_Survival);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTeammateCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentCrosshairRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentCommandTargetRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLastUsedPowerArmorRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveLastUsedPowerArmorRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAllMapMarkerRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActorsByProcessLevel);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActorsInHighProcess);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActorsInMiddleHighProcess);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActorsInMiddleLowProcess);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetActorsInLowProcess);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefsInRange);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllRefsInRangeFromRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLoadedCells);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentWorldSpace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPlayingHolotape);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, PlayHolotape);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, StopHolotape);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsPipBoyLightOn);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetIsPipBoyLightOn);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsTimeFrozen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsTimeFrozen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPlayerCharGenRace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPlayerCharGenRace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPerkPoints);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPerkPoints);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModPerkPoints);
	}
}
