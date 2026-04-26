#include "Plugin/Papyrus/Forms/LeveledList.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::LeveledList::Impl
{
	static constexpr auto OBJECT_KEY = "kObject"sv;
	static constexpr auto COUNT_KEY = "iCount"sv;
	static constexpr auto LEVEL_KEY = "iLevel"sv;
	static constexpr auto CHANCE_NONE_KEY = "iChanceNone"sv;

	static constexpr auto KEYWORD_KEY = "kKeyword"sv;
	static constexpr auto CHANCE_KEY = "iChance"sv;

	[[nodiscard]] static LeveledEntry GetLeveledEntry(const RE::LEVELED_OBJECT* a_object)
	{
		REX::Assert(a_object != nullptr);

		auto entry = LeveledEntry::Create();
		entry.Insert(OBJECT_KEY, a_object->form);
		entry.Insert(COUNT_KEY, a_object->count);
		entry.Insert(LEVEL_KEY, a_object->level);
		entry.Insert(CHANCE_NONE_KEY, a_object->chanceNone);

		return entry;
	}

	[[nodiscard]] static RE::LEVELED_OBJECT GetLeveledEditorEntry(const LeveledEntry& a_entry)
	{
		REX::Assert(a_entry != nullptr);

		auto* object = a_entry.Find<RE::TESForm*>(OBJECT_KEY).value();
		auto count = a_entry.Find<std::uint16_t>(COUNT_KEY).value();
		auto level = a_entry.Find<std::uint16_t>(LEVEL_KEY).value();
		auto chanceNone = a_entry.Find<std::int8_t>(CHANCE_NONE_KEY).value();

		return { object, level, count, chanceNone };
	}

	[[nodiscard]] static std::unique_ptr<RE::LEVELED_OBJECT> GetLeveledScriptEntry(const LeveledEntry& a_entry)
	{
		REX::Assert(a_entry != nullptr);

		auto object = GetLeveledEditorEntry(a_entry);
		return std::make_unique<RE::LEVELED_OBJECT>(std::move(object));
	}
}

namespace Plugin::Papyrus::Forms::LeveledList
{
	using LeveledListFlags = RE::TESLeveledList::Flags;

	static std::uint8_t GetMaxUseAllCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(leveledListHolder->maxUseAllCount).load(std::memory_order_acquire);
	}

	static bool SetMaxUseAllCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_value)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		std::atomic_ref(leveledListHolder->maxUseAllCount).store(a_value, std::memory_order_release);
		return true;
	}

	static std::int8_t GetChanceNone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(leveledListHolder->chanceNone).load(std::memory_order_acquire);
	}

	static bool SetChanceNone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::int8_t a_value)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		std::atomic_ref(leveledListHolder->chanceNone).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::TESGlobal* GetChanceNoneGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(leveledListHolder->chanceGlobal).load(std::memory_order_acquire);
	}

	static bool SetChanceNoneGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		RE::TESGlobal* a_value)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		std::atomic_ref(leveledListHolder->chanceGlobal).store(a_value, std::memory_order_release);
		return true;
	}

	static std::uint8_t GetEntryCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		return leveledListHolder->GetLevObjectCount();
	}

	static std::uint8_t GetEditorEntryCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		return leveledListHolder->GetEditorLevObjectCount();
	}

	static std::uint8_t GetScriptEntryCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return 0;
		}

		return leveledListHolder->GetScriptLevObjectCount();
	}

	static std::optional<std::vector<LeveledEntry>> GetAllEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<LeveledEntry>();
		result.reserve(leveledListHolder->GetLevObjectCount());

		leveledListHolder->ForEachLevObject([&result](RE::LEVELED_OBJECT* a_object) {
			if (!a_object) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			result.push_back(Impl::GetLeveledEntry(a_object));
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static std::optional<std::vector<LeveledEntry>> GetEditorEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<LeveledEntry>();
		result.reserve(leveledListHolder->GetEditorLevObjectCount());

		leveledListHolder->ForEachEditorLevObject([&result](RE::LEVELED_OBJECT* a_object) {
			if (!a_object) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			result.push_back(Impl::GetLeveledEntry(a_object));
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static bool SetEditorEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::vector<LeveledEntry> a_values)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto objects = std::vector<RE::LEVELED_OBJECT>();
		objects.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			objects.push_back(Impl::GetLeveledEditorEntry(entry));
		}

		leveledListHolder->SetEditorLevObjects(objects);
		return true;
	}

	static std::optional<std::vector<LeveledEntry>> GetScriptEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto result = std::vector<LeveledEntry>();
		result.reserve(leveledListHolder->GetScriptLevObjectCount());

		leveledListHolder->ForEachScriptLevObject([&result](RE::LEVELED_OBJECT* a_object) {
			if (!a_object) {
				return RE::BSContainer::ForEachResult::kContinue;
			}

			result.push_back(Impl::GetLeveledEntry(a_object));
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return result;
	}

	static bool SetScriptEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::vector<LeveledEntry> a_values)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto objects = std::vector<std::unique_ptr<RE::LEVELED_OBJECT>>();
		objects.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			objects.push_back(Impl::GetLeveledScriptEntry(entry));
		}

		leveledListHolder->SetScriptLevObjects(objects);
		return true;
	}

	static LeveledEntry GetNthEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}
		const auto* object = leveledListHolder->GetNthLevObject(a_index);
		if (!object) {
			return nullptr;
		}

		return Impl::GetLeveledEntry(object);
	}

	static bool SetNthEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index,
		LeveledEntry a_entry)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto object = Impl::GetLeveledEditorEntry(a_entry);
		return leveledListHolder->SetNthLevObject(a_index, object);
	}

	static bool RemoveNthEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		return leveledListHolder->RemoveNthLevObject(a_index);
	}

	static LeveledEntry GetNthEditorEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}
		const auto* object = leveledListHolder->GetNthEditorLevObject(a_index);
		if (!object) {
			return nullptr;
		}

		return Impl::GetLeveledEntry(object);
	}

	static bool SetNthEditorEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index,
		LeveledEntry a_entry)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto object = Impl::GetLeveledEditorEntry(a_entry);
		return leveledListHolder->SetNthEditorLevObject(a_index, object);
	}

	static bool RemoveNthEditorEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		return leveledListHolder->RemoveNthEditorLevObject(a_index);
	}

	static LeveledEntry GetNthScriptEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}
		const auto* object = leveledListHolder->GetNthScriptLevObject(a_index);
		if (!object) {
			return nullptr;
		}

		return Impl::GetLeveledEntry(object);
	}

	static bool SetNthScriptEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index,
		LeveledEntry a_entry)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto object = Impl::GetLeveledScriptEntry(a_entry);
		if (!object) {
			return false;
		}

		return leveledListHolder->SetNthScriptLevObject(a_index, std::move(object));
	}

	static bool RemoveNthScriptEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::uint8_t a_index)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		return leveledListHolder->RemoveNthScriptLevObject(a_index);
	}

	static bool AddEditorEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		LeveledEntry a_entry)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto object = Impl::GetLeveledEditorEntry(a_entry);
		return leveledListHolder->AddEditorLevObject(object);
	}

	static bool AddScriptEntry(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		LeveledEntry a_entry)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto object = Impl::GetLeveledScriptEntry(a_entry);
		if (!object) {
			return false;
		}

		return leveledListHolder->AddScriptLevObject(std::move(object));
	}

	static bool AddEditorEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::vector<LeveledEntry> a_values)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto objects = std::vector<RE::LEVELED_OBJECT>();
		objects.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			objects.push_back(Impl::GetLeveledEditorEntry(entry));
		}

		leveledListHolder->AddEditorLevObjects(objects);
		return true;
	}

	static bool AddScriptEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::vector<LeveledEntry> a_values)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto objects = std::vector<std::unique_ptr<RE::LEVELED_OBJECT>>();
		objects.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			objects.push_back(Impl::GetLeveledScriptEntry(entry));
		}

		leveledListHolder->AddScriptLevObjects(objects);
		return true;
	}

	static bool ClearEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		leveledListHolder->ClearLevObjects();
		return true;
	}

	static bool ClearEditorEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		leveledListHolder->ClearEditorLevObjects();
		return true;
	}

	static bool ClearScriptEntries(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		leveledListHolder->ClearScriptLevObjects();
		return true;
	}

	static std::optional<std::vector<KeywordEntry>> GetKeywordChances(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* keywordChances = leveledListHolder->keywordChances;
		if (!keywordChances) {
			return std::nullopt;
		}

		auto result = std::vector<KeywordEntry>();
		result.reserve(keywordChances->size());

		for (const auto& [keyword, chance] : *keywordChances) {
			auto entry = KeywordEntry::Create();
			entry.Insert(Impl::KEYWORD_KEY, keyword);
			entry.Insert(Impl::CHANCE_KEY, chance.int32);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static bool SetKeywordChances(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		std::vector<KeywordEntry> a_values)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto keywordChances = std::vector<std::pair<RE::BGSKeyword*, std::int32_t>>();
		keywordChances.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* keyword = entry.Find<RE::BGSKeyword*>(Impl::KEYWORD_KEY).value();
			auto chance = entry.Find<std::int32_t>(Impl::CHANCE_KEY).value();

			keywordChances.emplace_back(keyword, chance);
		}

		leveledListHolder->SetKeywordChances(keywordChances);
		return true;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		LeveledListFlags a_flag)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		return leveledListHolder->llFlags.any_atomic(a_flag);
	}

	static LeveledListFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return LeveledListFlags::kNone;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return LeveledListFlags::kNone;
		}

		return leveledListHolder->llFlags.get_atomic();
	}

	static bool SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		LeveledListFlags a_flags)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		leveledListHolder->llFlags.store_atomic(a_flags);
		return true;
	}

	static bool SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_leveledList,
		LeveledListFlags a_flag,
		bool a_set)
	{
		if (!a_leveledList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		auto* leveledListHolder = RE::DynamicCast<RE::TESLeveledList*>(a_leveledList);
		if (!leveledListHolder) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return false;
		}

		leveledListHolder->llFlags.set_atomic(a_set, a_flag);
		return true;
	}

	static LeveledListFlags GetFlag_CalculateAllLessEqualsPlayerLevel(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LeveledListFlags::kCalculateFromAllLevelsLTOrEqPCLevel; }
	static LeveledListFlags GetFlag_CalculateEachItemInCount(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LeveledListFlags::kCalculateForEachItemInCount; }
	static LeveledListFlags GetFlag_UseAll(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LeveledListFlags::kUseAll; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaxUseAllCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaxUseAllCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetChanceNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetChanceNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetChanceNoneGlobal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetChanceNoneGlobal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEntryCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEditorEntryCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptEntryCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAllEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEditorEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetEditorEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetScriptEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEditorEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEditorEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthEditorEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthScriptEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthScriptEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthScriptEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddEditorEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddScriptEntry);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddEditorEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddScriptEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearEditorEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearScriptEntries);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeywordChances);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetKeywordChances);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CalculateAllLessEqualsPlayerLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CalculateEachItemInCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseAll);
	}
}
