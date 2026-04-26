#include "Plugin/Papyrus/Forms/LeveledItem.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Forms::LeveledItem
{
	static RE::TESGlobal* GetLegendaryChanceGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESLevItem* a_leveledItem)
	{
		if (!a_leveledItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return nullptr;
		}

		const auto& legendaryLootChances = RE::TESLeveledList::GetSpecialChanceGlobalMap();

		const auto legendaryChanceIt = legendaryLootChances.find(a_leveledItem);
		if (legendaryChanceIt == legendaryLootChances.end()) {
			return nullptr;
		}

		return legendaryChanceIt->second;
	}

	static void SetLegendaryChanceGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESLevItem* a_leveledItem,
		RE::TESGlobal* a_value)
	{
		if (!a_leveledItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return;
		}

		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::GLOBAL_VARIABLE_NULL, a_stackId);
			return;
		}

		auto& legendaryLootChances = RE::TESLeveledList::GetSpecialChanceGlobalMap();
		legendaryLootChances.insert_or_assign(a_leveledItem, a_value);
	}

	static RE::BGSLocalizedString GetOverrideName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESLevItem* a_leveledItem)
	{
		if (!a_leveledItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return {};
		}

		const auto& overrideNames = RE::TESLevItem::GetOverrideNameMap();

		const auto nameIt = overrideNames.find(a_leveledItem);
		if (nameIt == overrideNames.end()) {
			return {};
		}

		return nameIt->second;
	}

	static void SetOverrideName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESLevItem* a_leveledItem,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_leveledItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LEVELED_LIST_NULL, a_stackId);
			return;
		}

		auto& overrideNames = RE::TESLevItem::GetOverrideNameMap();
		overrideNames.insert_or_assign(a_leveledItem, RE::BGSLocalizedString(a_value.GetValue()));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLegendaryChanceGlobal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetLegendaryChanceGlobal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetOverrideName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetOverrideName);
	}
}
