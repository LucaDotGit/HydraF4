#include "Plugin/Papyrus/Forms/Form.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Form::Parent::Impl
{
	static constexpr auto ITEM_KEY = "kItem"sv;
	static constexpr auto COUNT_KEY = "iCount"sv;
	static constexpr auto OWNER_KEY = "iOwner"sv;
	static constexpr auto OWNER_RANK_KEY = "iOwnerRank"sv;

	static constexpr auto VALUE_HOLDER_KEY = "kValueHolder"sv;
	static constexpr auto VALUE_KEY = "fValue"sv;
}

namespace Plugin::Papyrus::Forms::Form::Parent
{
	static std::uint32_t GetKeywordCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		return RE::TESForm::GetKeywordCount(a_form).value_or(0);
	}

	static std::int32_t GetKeywordIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return -1;
		}

		const auto index = RE::TESForm::GetKeywordIndex(a_form, a_keyword);
		if (!index.has_value()) {
			return -1;
		}

		return static_cast<std::int32_t>(index.value());
	}

	static std::optional<std::vector<RE::BGSKeyword*>> GetKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return std::nullopt;
		}

		return RE::TESForm::GetKeywords(a_form);
	}

	static bool SetKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BGSKeyword*> a_keywords)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::SetKeywords(a_form, a_keywords).value_or(false);
	}

	static RE::BGSKeyword* GetNthKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::uint32_t a_index)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		return RE::TESForm::GetNthKeyword(a_form, a_index).value_or(nullptr);
	}

	static bool SetNthKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::uint32_t a_index,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::SetNthKeyword(a_form, a_index, a_keyword).value_or(false);
	}

	static bool RemoveNthKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::uint32_t a_index)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::RemoveNthKeyword(a_form, a_index).value_or(false);
	}

	static bool AddKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::AddKeyword(a_form, a_keyword).value_or(false);
	}

	static bool RemoveKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::RemoveKeyword(a_form, a_keyword).value_or(false);
	}

	static bool ReplaceKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_oldKeyword,
		RE::BGSKeyword* a_newKeyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::ReplaceKeyword(a_form, a_oldKeyword, a_newKeyword).value_or(false);
	}

	static bool AddKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BGSKeyword*> a_keywords)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::AddKeywords(a_form, a_keywords);
	}

	static bool RemoveKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BGSKeyword*> a_keywords)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::RemoveKeywords(a_form, a_keywords);
	}

	static bool ClearKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::ClearKeywords(a_form);
	}

	static RE::BGSLocalizedString GetName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		auto name = RE::TESFullName::GetFormFullOrDisplayName(a_form);
		if (!name.has_value()) {
			return {};
		}

		return std::move(name).value();
	}

	static bool SetName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSLocalizedString a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESFullName::SetFormFullOrDisplayName(a_form, a_value);
	}

	static RE::BSFixedStringCS GetDescription(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		auto description = RE::TESDescription::GetFormDescription(a_form);
		if (!description.has_value()) {
			return {};
		}

		return std::move(description).value();
	}

	static std::int32_t GetValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		const auto value = RE::TESValueForm::GetFormValue(a_form);
		if (!value.has_value()) {
			return 0;
		}

		return value.value();
	}

	static bool SetValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::int32_t a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESValueForm::SetFormValue(a_form, a_value);
	}

	static REX::Float32 GetWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto weight = RE::TESWeightForm::GetFormWeight(a_form);
		if (!weight.has_value()) {
			return 0.0_f32;
		}

		return weight.value();
	}

	static bool SetWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		REX::Float32 a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESWeightForm::SetFormWeight(a_form, a_value);
	}

	static RE::TESRace* GetRace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* raceHolder = RE::DynamicCast<const RE::TESRaceForm*>(a_form);
		if (!raceHolder) {
			return nullptr;
		}

		return std::atomic_ref(raceHolder->formRace).load(std::memory_order_acquire);
	}

	static bool SetRace(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::TESRace* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* raceHolder = RE::DynamicCast<RE::TESRaceForm*>(a_form);
		if (!raceHolder) {
			return false;
		}

		std::atomic_ref(raceHolder->formRace).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::TESObjectARMO* GetSkin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* skinHolder = RE::DynamicCast<const RE::BGSSkinForm*>(a_form);
		if (!skinHolder) {
			return nullptr;
		}

		return std::atomic_ref(skinHolder->formSkin).load(std::memory_order_acquire);
	}

	static bool SetSkin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::TESObjectARMO* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* skinHolder = RE::DynamicCast<RE::BGSSkinForm*>(a_form);
		if (!skinHolder) {
			return false;
		}

		std::atomic_ref(skinHolder->formSkin).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSEquipSlot* GetEquipType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* type = RE::DynamicCast<const RE::BGSEquipType*>(a_form);
		if (!type) {
			return nullptr;
		}

		return std::atomic_ref(type->equipSlot).load(std::memory_order_acquire);
	}

	static bool SetEquipType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSEquipSlot* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* type = RE::DynamicCast<RE::BGSEquipType*>(a_form);
		if (!type) {
			return false;
		}

		std::atomic_ref(type->equipSlot).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::EnchantmentItem* GetEnchantment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* enchantmentHolder = RE::DynamicCast<const RE::TESEnchantableForm*>(a_form);
		if (!enchantmentHolder) {
			return nullptr;
		}

		return std::atomic_ref(enchantmentHolder->formEnchanting).load(std::memory_order_acquire);
	}

	static bool SetEnchantment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::EnchantmentItem* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* enchantmentHolder = RE::DynamicCast<RE::TESEnchantableForm*>(a_form);
		if (!enchantmentHolder) {
			return false;
		}

		std::atomic_ref(enchantmentHolder->formEnchanting).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSInstanceNamingRules* GetInstanceNamingRules(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* namingRulesHolder = RE::DynamicCast<const RE::BGSInstanceNamingRulesForm*>(a_form);
		if (!namingRulesHolder) {
			return nullptr;
		}

		return std::atomic_ref(namingRulesHolder->instanceNamingRules).load(std::memory_order_acquire);
	}

	static bool SetInstanceNamingRules(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSInstanceNamingRules* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* namingRulesHolder = RE::DynamicCast<RE::BGSInstanceNamingRulesForm*>(a_form);
		if (!namingRulesHolder) {
			return false;
		}

		std::atomic_ref(namingRulesHolder->instanceNamingRules).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSImpactDataSet* GetBlockBashImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* impactDataHolder = RE::DynamicCast<const RE::BGSBlockBashData*>(a_form);
		if (!impactDataHolder) {
			return nullptr;
		}

		return std::atomic_ref(impactDataHolder->blockBashImpactDataSet).load(std::memory_order_acquire);
	}

	static bool SetBlockBashImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSImpactDataSet* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* impactDataHolder = RE::DynamicCast<RE::BGSBlockBashData*>(a_form);
		if (!impactDataHolder) {
			return false;
		}

		std::atomic_ref(impactDataHolder->blockBashImpactDataSet).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSMaterialType* GetBlockBashAltMaterialType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* impactDataHolder = RE::DynamicCast<const RE::BGSBlockBashData*>(a_form);
		if (!impactDataHolder) {
			return nullptr;
		}

		return std::atomic_ref(impactDataHolder->altBlockMaterialType).load(std::memory_order_acquire);
	}

	static bool SetBlockBashAltMaterialType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSMaterialType* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* impactDataHolder = RE::DynamicCast<RE::BGSBlockBashData*>(a_form);
		if (!impactDataHolder) {
			return false;
		}

		std::atomic_ref(impactDataHolder->altBlockMaterialType).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSSoundDescriptorForm* GetPickUpSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* soundHolder = RE::DynamicCast<const RE::BGSPickupPutdownSounds*>(a_form);
		if (!soundHolder) {
			return nullptr;
		}

		return std::atomic_ref(soundHolder->pickupSound).load(std::memory_order_acquire);
	}

	static bool SetPickUpSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* soundHolder = RE::DynamicCast<RE::BGSPickupPutdownSounds*>(a_form);
		if (!soundHolder) {
			return false;
		}

		std::atomic_ref(soundHolder->pickupSound).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSSoundDescriptorForm* GetPutDownSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* soundHolder = RE::DynamicCast<const RE::BGSPickupPutdownSounds*>(a_form);
		if (!soundHolder) {
			return nullptr;
		}

		return std::atomic_ref(soundHolder->putdownSound).load(std::memory_order_acquire);
	}

	static bool SetPutDownSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* soundHolder = RE::DynamicCast<RE::BGSPickupPutdownSounds*>(a_form);
		if (!soundHolder) {
			return false;
		}

		std::atomic_ref(soundHolder->putdownSound).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSSoundDescriptorForm* GetCraftingSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* soundHolder = RE::DynamicCast<const RE::BGSCraftingUseSound*>(a_form);
		if (!soundHolder) {
			return nullptr;
		}

		return std::atomic_ref(soundHolder->craftingUseSound).load(std::memory_order_acquire);
	}

	static bool SetCraftingSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* soundHolder = RE::DynamicCast<RE::BGSCraftingUseSound*>(a_form);
		if (!soundHolder) {
			return false;
		}

		std::atomic_ref(soundHolder->craftingUseSound).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSTerminal* GetTerminal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* terminalHolder = RE::DynamicCast<const RE::BGSNativeTerminalForm*>(a_form);
		if (!terminalHolder) {
			return nullptr;
		}

		return std::atomic_ref(terminalHolder->terminal).load(std::memory_order_acquire);
	}

	static bool SetTerminal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSTerminal* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* terminalHolder = RE::DynamicCast<RE::BGSNativeTerminalForm*>(a_form);
		if (!terminalHolder) {
			return false;
		}

		std::atomic_ref(terminalHolder->terminal).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSLocationRefType* GetForcedLocationRefType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* locationRefTypeHolder = RE::DynamicCast<const RE::BGSForcedLocRefType*>(a_form);
		if (!locationRefTypeHolder) {
			return nullptr;
		}

		return std::atomic_ref(locationRefTypeHolder->forcedType).load(std::memory_order_acquire);
	}

	static bool SetForcedLocationRefType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSLocationRefType* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* locationRefTypeHolder = RE::DynamicCast<RE::BGSForcedLocRefType*>(a_form);
		if (!locationRefTypeHolder) {
			return false;
		}

		std::atomic_ref(locationRefTypeHolder->forcedType).store(a_value, std::memory_order_release);
		return true;
	}

	static RE::BGSMaterialSwap* GetMaterialSwap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto* matSwapHolder = RE::DynamicCast<const RE::BGSModelMaterialSwap*>(a_form);
		if (!matSwapHolder) {
			return nullptr;
		}

		return std::atomic_ref(matSwapHolder->swapForm).load(std::memory_order_acquire);
	}

	static bool SetMaterialSwap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSMaterialSwap* a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* matSwapHolder = RE::DynamicCast<RE::BGSModelMaterialSwap*>(a_form);
		if (!matSwapHolder) {
			return false;
		}

		std::atomic_ref(matSwapHolder->swapForm).store(a_value, std::memory_order_release);
		return true;
	}

	static REX::Float32 GetMaterialSwapColorRemapIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		const auto* matSwapHolder = RE::DynamicCast<const RE::BGSModelMaterialSwap*>(a_form);
		if (!matSwapHolder) {
			return 0;
		}

		return std::atomic_ref(matSwapHolder->colorRemappingIndex).load(std::memory_order_acquire);
	}

	static bool SetMaterialSwapColorRemapIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		REX::Float32 a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* matSwapHolder = RE::DynamicCast<RE::BGSModelMaterialSwap*>(a_form);
		if (!matSwapHolder) {
			return false;
		}

		std::atomic_ref(matSwapHolder->colorRemappingIndex).store(a_value, std::memory_order_release);
		return true;
	}

	static std::optional<std::vector<ContainerEntry>> GetContainerItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* container = RE::DynamicCast<const RE::TESContainer*>(a_form);
		if (!container) {
			return std::nullopt;
		}

		const auto items = container->GetObjects();

		auto result = std::vector<ContainerEntry>();
		result.reserve(items.size());

		for (const auto* item : items) {
			if (!item) {
				continue;
			}

			auto entry = ContainerEntry::Create();
			entry.Insert(Impl::ITEM_KEY, item->obj);
			entry.Insert(Impl::COUNT_KEY, item->count);
			entry.Insert(Impl::OWNER_KEY, item->itemExtra ? item->itemExtra->ownerForm : nullptr);
			entry.Insert(Impl::OWNER_RANK_KEY, item->itemExtra ? item->itemExtra->conditional.ownerRank : 0);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static bool SetContainerItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<ContainerEntry> a_values)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* container = RE::DynamicCast<RE::TESContainer*>(a_form);
		if (!container) {
			return false;
		}

		auto items = std::vector<std::unique_ptr<RE::ContainerObject>>();
		items.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* item = entry.Find<RE::TESBoundObject*>(Impl::ITEM_KEY).value();
			auto count = entry.Find<std::int32_t>(Impl::COUNT_KEY).value();
			auto* owner = entry.Find<RE::TESForm*>(Impl::OWNER_KEY).value();
			auto ownerRank = entry.Find<std::int32_t>(Impl::OWNER_RANK_KEY).value();

			if (!item) {
				continue;
			}

			items.push_back(std::make_unique<RE::ContainerObject>(item, count, owner, ownerRank));
		}

		container->SetObjects(items);
		return true;
	}

	static std::optional<std::vector<PropertyEntry>> GetProperties(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* propertyHolder = RE::DynamicCast<const RE::BGSPropertySheet*>(a_form);
		if (!propertyHolder) {
			return std::nullopt;
		}

		const auto properties = propertyHolder->GetProperties();
		if (!properties.has_value()) {
			return std::nullopt;
		}

		auto result = std::vector<PropertyEntry>();
		result.reserve(properties->size());

		for (const auto& [object, value] : *properties) {
			if (!object) {
				continue;
			}

			auto entry = PropertyEntry::Create();
			entry.Insert(Impl::VALUE_HOLDER_KEY, RE::DynamicCast<RE::ActorValueInfo*>(object));
			entry.Insert(Impl::VALUE_KEY, value.float32);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static bool SetProperties(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<PropertyEntry> a_values)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		auto* propertyHolder = RE::DynamicCast<RE::BGSPropertySheet*>(a_form);
		if (!propertyHolder) {
			return false;
		}

		auto properties = std::vector<RE::BSTPair<RE::TESForm*, RE::BGSTypedFormValuePair::SharedVal>>();
		properties.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* valueHolder = entry.Find<RE::ActorValueInfo*>(Impl::VALUE_HOLDER_KEY).value();
			auto value = entry.Find<REX::Float32>(Impl::VALUE_KEY).value();

			properties.emplace_back(valueHolder, value);
		}

		propertyHolder->SetProperties(properties);
		return true;
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeywordCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeywordIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeywords);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetKeywords);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ReplaceKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddKeywords);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveKeywords);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearKeywords);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDescription);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSkin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSkin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEquipType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEquipType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEnchantment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEnchantment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInstanceNamingRules);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetInstanceNamingRules);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBlockBashImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBlockBashImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBlockBashAltMaterialType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBlockBashAltMaterialType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPickUpSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPickUpSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPutDownSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPutDownSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCraftingSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCraftingSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTerminal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTerminal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetForcedLocationRefType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetForcedLocationRefType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaterialSwap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaterialSwap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaterialSwapColorRemapIndex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaterialSwapColorRemapIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetContainerItems);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetContainerItems);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetProperties);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetProperties);
	}
}
