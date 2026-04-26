#include "Plugin/Papyrus/Forms/Perk.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Perk::Impl
{
	static constexpr auto RANK_KEY = "iRank"sv;
	static constexpr auto PRIORITY_KEY = "iPriority"sv;

	static constexpr auto QUEST_KEY = "kQuest"sv;
	static constexpr auto STAGE_KEY = "iStage"sv;

	static constexpr auto VALUE_ONE_KEY = "fValue01"sv;
	static constexpr auto VALUE_TWO_KEY = "fValue02"sv;

	static constexpr auto ACTOR_VALUE_KEY = "kActorValue"sv;
	static constexpr auto VALUE_KEY = "fValue"sv;
}

namespace Plugin::Papyrus::Forms::Perk
{
	using PerkFunctionType = RE::EntryPointFunctionType;
	using PerkEntryType = RE::PERK_ENTRY_TYPE;

	static bool GetIsTrait(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_perk->data.trait).load(std::memory_order_acquire);
	}

	static void SetIsTrait(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		bool a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->data.trait).store(a_value, std::memory_order_release);
	}

	static bool GetIsPlayable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_perk->data.playable).load(std::memory_order_acquire);
	}

	static void SetIsPlayable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		bool a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->data.playable).store(a_value, std::memory_order_release);
	}

	static bool GetIsHidden(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_perk->data.hidden).load(std::memory_order_acquire);
	}

	static void SetIsHidden(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		bool a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->data.hidden).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_perk->data.level).load(std::memory_order_acquire);
	}

	static void SetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::uint8_t a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->data.level).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetRankCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_perk->data.numRanks).load(std::memory_order_acquire);
	}

	static void SetRankCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::uint8_t a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->data.numRanks).store(a_value, std::memory_order_release);
	}

	static RE::BGSPerk* GetNextPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_perk->nextPerk).load(std::memory_order_acquire);
	}

	static void SetNextPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		RE::BGSPerk* a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->nextPerk).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_perk->sound).load(std::memory_order_acquire);
	}

	static void SetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_perk->sound).store(a_value, std::memory_order_release);
	}

	static RE::BSFixedStringCS GetUIFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return {};
		}

		return a_perk->swfFile;
	}

	static void SetUIFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		RE::BSFixedStringCS a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		a_perk->swfFile = std::move(a_value);
	}

	static std::optional<std::vector<Entry>> GetEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& perkEntries = a_perk->perkEntries;

		auto result = std::vector<Entry>();
		result.reserve(perkEntries.size());

		for (const auto* perkEntry : a_perk->perkEntries) {
			if (!perkEntry) {
				continue;
			}

			auto entry = Entry::Create();
			entry.Insert(Impl::RANK_KEY, perkEntry->rank);
			entry.Insert(Impl::PRIORITY_KEY, perkEntry->priority);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::vector<Entry> a_values)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = std::min(perkEntries.size(), static_cast<std::uint32_t>(a_values.size()));
		for (auto i = 0ui32; i < maxIndex; i++) {
			const auto& entry = a_values[i];
			if (!entry) {
				continue;
			}

			auto* perkEntry = perkEntries[i];
			if (!perkEntry) {
				continue;
			}

			perkEntry->rank = entry.Find<std::uint8_t>(Impl::RANK_KEY).value();
			perkEntry->priority = entry.Find<std::uint8_t>(Impl::PRIORITY_KEY).value();
		}
	}

	static PerkEntryType GetNthEntryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return PerkEntryType::kInvalid;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return PerkEntryType::kInvalid;
		}

		const auto* perkEntry = perkEntries[a_index];
		return perkEntry->GetType();
	}

	static PerkEntryType GetEntryType_Invalid(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkEntryType::kInvalid; }
	static PerkEntryType GetEntryType_Quest(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkEntryType::kQuest; }
	static PerkEntryType GetEntryType_Ability(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkEntryType::kAbility; }
	static PerkEntryType GetEntryType_EntryPoint(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkEntryType::kEntryPoint; }

	static PerkFunctionType GetNthEntryFunctionType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return PerkFunctionType::kInvalid;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return PerkFunctionType::kInvalid;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);
		if (!perkEntryPoint || !perkEntryPoint->functionData) {
			return PerkFunctionType::kInvalid;
		}

		return perkEntryPoint->functionData->GetType();
	}

	static PerkFunctionType GetEntryFunctionType_Invalid(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kInvalid; }
	static PerkFunctionType GetEntryFunctionType_OneValue(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kOneValue; }
	static PerkFunctionType GetEntryFunctionType_TwoValue(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kTwoValue; }
	static PerkFunctionType GetEntryFunctionType_LeveledList(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kLeveledList; }
	static PerkFunctionType GetEntryFunctionType_ActivateChoice(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kActivateChoice; }
	static PerkFunctionType GetEntryFunctionType_Spell(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kSpellItem; }
	static PerkFunctionType GetEntryFunctionType_BooleanGraphVariable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kBooleanGraphVariable; }
	static PerkFunctionType GetEntryFunctionType_Text(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kText; }
	static PerkFunctionType GetEntryFunctionType_ActorValue(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PerkFunctionType::kActorValue; }

	static QuestEntry GetNthQuestEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return nullptr;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkQuestEntry = RE::DynamicCast<const RE::BGSQuestPerkEntry*>(perkEntry);

		if (!perkQuestEntry) {
			return nullptr;
		}

		auto entry = QuestEntry::Create();
		entry.Insert(Impl::QUEST_KEY, perkQuestEntry->data.quest);
		entry.Insert(Impl::STAGE_KEY, perkQuestEntry->data.stage);

		return entry;
	}

	static bool SetNthQuestEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		QuestEntry a_entry)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* questEntry = RE::DynamicCast<RE::BGSQuestPerkEntry*>(perkEntry);

		if (!questEntry) {
			return false;
		}

		questEntry->data.quest = a_entry.Find<RE::TESQuest*>(Impl::QUEST_KEY).value();
		questEntry->data.stage = a_entry.Find<std::uint16_t>(Impl::STAGE_KEY).value();
		return true;
	}

	static REX::Float32 GetNthEntryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return 0.0_f32;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return 0.0_f32;
		}

		const auto* functionOne = RE::DynamicCast<const RE::BGSEntryPointFunctionDataOneValue*>(perkEntryPoint->functionData);
		if (!functionOne) {
			return 0.0_f32;
		}

		return functionOne->value;
	}

	static bool SetNthEntryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		REX::Float32 a_value)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return false;
		}

		auto* functionOne = RE::DynamicCast<RE::BGSEntryPointFunctionDataOneValue*>(perkEntryPoint->functionData);
		if (!functionOne) {
			return false;
		}

		functionOne->value = a_value;
		return true;
	}

	static ValueTwoEntry GetNthValueTwoEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return nullptr;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return nullptr;
		}

		const auto* functionTwo = RE::DynamicCast<const RE::BGSEntryPointFunctionDataTwoValue*>(perkEntryPoint->functionData);
		if (!functionTwo) {
			return nullptr;
		}

		auto entry = ValueTwoEntry::Create();
		entry.Insert(Impl::VALUE_ONE_KEY, functionTwo->value01);
		entry.Insert(Impl::VALUE_TWO_KEY, functionTwo->value02);

		return entry;
	}

	static bool SetNthValueTwoEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		ValueTwoEntry a_entry)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return false;
		}

		auto* functionTwo = RE::DynamicCast<RE::BGSEntryPointFunctionDataTwoValue*>(perkEntryPoint->functionData);
		if (!functionTwo) {
			return false;
		}

		functionTwo->value01 = a_entry.Find<REX::Float32>(Impl::VALUE_ONE_KEY).value();
		functionTwo->value02 = a_entry.Find<REX::Float32>(Impl::VALUE_TWO_KEY).value();
		return true;
	}

	static RE::TESLevItem* GetNthEntryLeveledList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return nullptr;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return nullptr;
		}

		const auto* functionData = RE::DynamicCast<const RE::BGSEntryPointFunctionDataLeveledList*>(perkEntryPoint->functionData);
		if (!functionData) {
			return nullptr;
		}

		return functionData->levList;
	}

	static bool SetNthEntryLeveledList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		RE::TESLevItem* a_leveledList)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return false;
		}

		auto* functionData = RE::DynamicCast<RE::BGSEntryPointFunctionDataLeveledList*>(perkEntryPoint->functionData);
		if (!functionData) {
			return false;
		}

		functionData->levList = a_leveledList;
		return true;
	}

	static RE::SpellItem* GetNthEntrySpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return nullptr;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* abilityEntry = RE::DynamicCast<const RE::BGSAbilityPerkEntry*>(perkEntry);

		if (abilityEntry) {
			return abilityEntry->ability;
		}

		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);
		if (!perkEntryPoint) {
			return nullptr;
		}

		const auto* functionDataSpell = RE::DynamicCast<const RE::BGSEntryPointFunctionDataSpellItem*>(perkEntryPoint->functionData);
		if (functionDataSpell) {
			return functionDataSpell->spell;
		}

		const auto* functionDataActivation = RE::DynamicCast<const RE::BGSEntryPointFunctionDataActivateChoice*>(perkEntryPoint->functionData);
		if (functionDataActivation) {
			return functionDataActivation->spell;
		}

		return nullptr;
	}

	static bool SetNthEntrySpell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		RE::SpellItem* a_spell)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* abilityEntry = RE::DynamicCast<RE::BGSAbilityPerkEntry*>(perkEntry);

		if (abilityEntry) {
			abilityEntry->ability = a_spell;
			return true;
		}

		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);
		if (!perkEntryPoint) {
			return false;
		}

		auto* functionDataSpell = RE::DynamicCast<RE::BGSEntryPointFunctionDataSpellItem*>(perkEntryPoint->functionData);
		if (functionDataSpell) {
			functionDataSpell->spell = a_spell;
			return true;
		}

		auto* functionDataActivation = RE::DynamicCast<RE::BGSEntryPointFunctionDataActivateChoice*>(perkEntryPoint->functionData);
		if (functionDataActivation) {
			functionDataActivation->spell = a_spell;
			return true;
		}

		return false;
	}

	static RE::BSFixedString GetNthEntryText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return {};
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return {};
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return {};
		}

		const auto* functionData = RE::DynamicCast<const RE::BGSEntryPointFunctionDataText*>(perkEntryPoint->functionData);
		if (functionData) {
			return RE::BSFixedString(functionData->text);
		}

		const auto* functionDataActivation = RE::DynamicCast<const RE::BGSEntryPointFunctionDataActivateChoice*>(perkEntryPoint->functionData);
		if (functionDataActivation) {
			return RE::BSFixedString(functionDataActivation->label);
		}

		const auto* functionDataGraph = RE::DynamicCast<const RE::BGSEntryPointFunctionDataBooleanGraphVariable*>(perkEntryPoint->functionData);
		if (functionDataGraph) {
			return RE::BSFixedString(functionDataGraph->variable);
		}

		return {};
	}

	static bool SetNthEntryText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		RE::BSFixedString a_text)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return false;
		}

		auto* functionData = RE::DynamicCast<RE::BGSEntryPointFunctionDataText*>(perkEntryPoint->functionData);
		if (functionData) {
			functionData->text = a_text;
			return true;
		}

		auto* functionDataActivation = RE::DynamicCast<RE::BGSEntryPointFunctionDataActivateChoice*>(perkEntryPoint->functionData);
		if (functionDataActivation) {
			functionDataActivation->label = a_text;
			return true;
		}

		auto* functionDataGraph = RE::DynamicCast<RE::BGSEntryPointFunctionDataBooleanGraphVariable*>(perkEntryPoint->functionData);
		if (functionDataGraph) {
			functionDataGraph->variable = a_text;
			return true;
		}

		return false;
	}

	static ActorValueEntry GetNthActorValueEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
			return nullptr;
		}

		const auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return nullptr;
		}

		const auto* perkEntry = perkEntries[a_index];
		const auto* perkEntryPoint = RE::DynamicCast<const RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return nullptr;
		}

		auto* functionData = RE::DynamicCast<RE::BGSEntryPointFunctionDataAVAndValue*>(perkEntryPoint->functionData);
		if (!functionData) {
			return nullptr;
		}

		auto entry = ActorValueEntry::Create();
		entry.Insert(Impl::ACTOR_VALUE_KEY, functionData->actorValue);
		entry.Insert(Impl::VALUE_KEY, functionData->value);

		return entry;
	}

	static bool SetNthActorValueEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSPerk* a_perk,
		std::int32_t a_index,
		ActorValueEntry a_entry)
	{
		if (!a_perk) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PERK_NULL, a_stackId);
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto& perkEntries = a_perk->perkEntries;

		const auto maxIndex = static_cast<std::int32_t>(perkEntries.size() - 1);
		if (a_index < 0 || a_index > maxIndex) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, maxIndex);
			return false;
		}

		auto* perkEntry = perkEntries[a_index];
		auto* perkEntryPoint = RE::DynamicCast<RE::BGSEntryPointPerkEntry*>(perkEntry);

		if (!perkEntryPoint) {
			return false;
		}

		auto* functionData = RE::DynamicCast<RE::BGSEntryPointFunctionDataAVAndValue*>(perkEntryPoint->functionData);
		if (!functionData) {
			return false;
		}

		functionData->actorValue = a_entry.Find<RE::ActorValueInfo*>(Impl::ACTOR_VALUE_KEY).value();
		functionData->value = a_entry.Find<REX::Float32>(Impl::VALUE_KEY).value();
		return true;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsTrait);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsTrait);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsPlayable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsPlayable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsHidden);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsHidden);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRankCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRankCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNextPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetNextPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetUIFilePath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetUIFilePath);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntryType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntryFunctionType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryType_Invalid);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryType_Quest);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryType_Ability);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryType_EntryPoint);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_Invalid);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_OneValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_TwoValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_LeveledList);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_ActivateChoice);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_Spell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_BooleanGraphVariable);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_Text);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryFunctionType_ActorValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthQuestEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthQuestEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntryValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEntryValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthValueTwoEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthValueTwoEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntryLeveledList);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEntryLeveledList);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntrySpell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEntrySpell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntryText);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEntryText);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthActorValueEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthActorValueEntry);
	}
}
