#include "Plugin/Papyrus/Saves.hpp"

#include "Plugin/Internal/Functors/AutoSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/CustomSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/FullSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/LoadSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/QuickLoadDelayFunctor.hpp"
#include "Plugin/Internal/Functors/QuickSaveDelayFunctor.hpp"
#include "Plugin/Internal/Game/GameSaveSettings.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::Saves
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;

	static bool IsLoadingAllowed(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return saveLoadManager->IsLoadingAllowed();
	}

	static bool IsSavingAllowed(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return saveLoadManager->IsSavingAllowed();
	}

	static bool IsAutoSavingAllowed(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return saveLoadManager->IsAutoSavingAllowed();
	}

	static bool IsSaveExistent(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_fileName)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return saveLoadManager->IsSaveFileExistent(a_fileName.data());
	}

	static RE::BSFixedString GetCurrentSaveName(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		const auto* mostRecentSaveGame = saveLoadManager->mostRecentSaveGame;
		if (REX::IsNullOrEmpty(mostRecentSaveGame)) {
			return {};
		}

		return { mostRecentSaveGame };
	}

	static std::optional<LongStruct> GetCurrentProfileId(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		return LongStruct::FromUInt64(saveLoadManager->currentPlayerID);
	}

	static RE::BSFixedString GetCurrentProfileName(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		const auto currentPlayerId = saveLoadManager->currentPlayerID;
		const auto& playerIdNames = saveLoadManager->playerIDNameMap;

		const auto playerNameIt = playerIdNames.find(currentPlayerId);
		if (playerNameIt == playerIdNames.end()) {
			return {};
		}

		return playerNameIt->second;
	}

	static std::uint32_t GetCurrentSaveNumber(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return saveLoadManager->currentSaveGameNumber;
	}

	static std::uint32_t GetCurrentAutoSaveNumber(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return saveLoadManager->currentAutoSaveNumber;
	}

	static RE::BSScript::LatentResult<bool> LoadSave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_fileName,
		bool a_silent)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!saveLoadManager->IsLoadingAllowed() || !saveLoadManager->IsSaveFileExistent(a_fileName.data())) {
			return a_vm.ReturnLatentResult(a_stackId, false);
		}

		const auto loadFunctor = ::Plugin::Internal::Functors::LoadSaveDelayFunctorFactory::GetSingleton()->CreateObject(a_fileName, a_stackId, a_silent);
		F4SE::GetDelayFunctorManager()->Enqueue(loadFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<bool> QuickLoad(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto quickSaveFileName = saveLoadManager->quicksaveFileName;
		if (!saveLoadManager->IsLoadingAllowed() || quickSaveFileName.empty()) {
			return a_vm.ReturnLatentResult(a_stackId, false);
		}

		const auto loadFunctor = ::Plugin::Internal::Functors::QuickLoadDelayFunctorFactory::GetSingleton()->CreateObject(saveLoadManager->quicksaveFileName, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(loadFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSFixedString> CustomSave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_fileName,
		bool a_silent)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!saveLoadManager->IsSavingAllowed() || saveLoadManager->IsSaveFileExistent(a_fileName.data())) {
			return a_vm.ReturnLatentResult(a_stackId, RE::BSFixedString());
		}

		const auto saveFunctor = ::Plugin::Internal::Functors::CustomSaveDelayFunctorFactory::GetSingleton()->CreateObject(a_fileName, a_silent, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(saveFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSFixedString> FullSave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!saveLoadManager->IsSavingAllowed()) {
			return a_vm.ReturnLatentResult(a_stackId, RE::BSFixedString());
		}

		const auto saveFunctor = ::Plugin::Internal::Functors::FullSaveDelayFunctorFactory::GetSingleton()->CreateObject(a_silent, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(saveFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSFixedString> QuickSave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!saveLoadManager->IsSavingAllowed()) {
			return a_vm.ReturnLatentResult(a_stackId, RE::BSFixedString());
		}

		const auto saveFunctor = ::Plugin::Internal::Functors::QuickSaveDelayFunctorFactory::GetSingleton()->CreateObject(a_silent, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(saveFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSFixedString> AutoSave(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		const auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!saveLoadManager->IsAutoSavingAllowed()) {
			return a_vm.ReturnLatentResult(a_stackId, RE::BSFixedString());
		}

		const auto saveFunctor = ::Plugin::Internal::Functors::AutoSaveDelayFunctorFactory::GetSingleton()->CreateObject(a_silent, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(saveFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	static bool CustomSaveAsync(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_fileName,
		bool a_silent)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!saveLoadManager->IsSavingAllowed() || saveLoadManager->IsSaveFileExistent(a_fileName.data())) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([saveLoadManager, a_fileName = std::move(a_fileName), a_silent]() {
			saveLoadManager->CreateCompleteCustomSave(a_fileName.data(), a_silent);
		});

		return true;
	}

	static bool FullSaveAsync(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!saveLoadManager->IsSavingAllowed()) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([saveLoadManager, a_silent]() {
			saveLoadManager->CreateCompleteFullSave(a_silent);
		});

		return true;
	}

	static bool QuickSaveAsync(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!saveLoadManager->IsSavingAllowed()) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([saveLoadManager, a_silent]() {
			saveLoadManager->CreateCompleteQuicksave(a_silent);
		});

		return true;
	}

	static bool AutoSaveAsync(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_silent)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!saveLoadManager->IsAutoSavingAllowed()) {
			return false;
		}

		F4SE::GetTaskInterface()->AddTask([saveLoadManager, a_silent]() {
			saveLoadManager->CreateCompleteAutosave(a_silent);
		});

		return true;
	}

	static bool DeleteSave(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_fileName)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return saveLoadManager->DeleteSaveFile(a_fileName.data());
	}

	static REX::Float32 GetLoadInFadeDuration(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetLoadInFadeDuration();
	}

	static void SetLoadInFadeDuration(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetLoadInFadeDuration(a_value);
	}

	static bool GetEnableAutoSaves(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetEnableAutoSaves();
	}

	static void SetEnableAutoSaves(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetEnableAutoSaves(a_value);
	}

	static bool GetDoSaveOnWorkshopExit(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetDoSaveOnWorkshopExit();
	}

	static void SetDoSaveOnWorkshopExit(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetDoSaveOnWorkshopExit(a_value);
	}

	static bool GetDoSaveOnFastTravel(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetDoSaveOnFastTravel();
	}

	static void SetDoSaveOnFastTravel(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetDoSaveOnFastTravel(a_value);
	}

	static bool GetDoSaveOnRest(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetDoSaveOnRest();
	}

	static void SetDoSaveOnRest(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetDoSaveOnRest(a_value);
	}

	static bool GetDoSaveOnWait(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetDoSaveOnWait();
	}

	static void SetDoSaveOnWait(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetDoSaveOnWait(a_value);
	}

	static std::int32_t GetMaxAutoSaveCount(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->GetMaxAutoSaveCount();
	}

	static void SetMaxAutoSaveCount(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetSaveSettings()->SetMaxAutoSaveCount(a_value);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsLoadingAllowed);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsSavingAllowed);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsAutoSavingAllowed);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsSaveExistent);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentSaveName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentProfileId);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentProfileName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentSaveNumber);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentAutoSaveNumber);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, LoadSave);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, QuickLoad);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, CustomSave);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, FullSave);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, QuickSave);
		RE_REGISTER_VM_FUNCTION_LATENT(a_vm, SCRIPT_NAME, AutoSave);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, CustomSaveAsync);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FullSaveAsync);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, QuickSaveAsync);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AutoSaveAsync);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, DeleteSave);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLoadInFadeDuration);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLoadInFadeDuration);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEnableAutoSaves);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetEnableAutoSaves);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDoSaveOnWorkshopExit);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDoSaveOnWorkshopExit);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDoSaveOnFastTravel);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDoSaveOnFastTravel);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDoSaveOnRest);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDoSaveOnRest);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetDoSaveOnWait);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetDoSaveOnWait);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetMaxAutoSaveCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetMaxAutoSaveCount);
	}
}
