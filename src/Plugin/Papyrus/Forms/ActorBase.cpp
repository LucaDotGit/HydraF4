#include "Plugin/Papyrus/Forms/ActorBase.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::ActorBase::Impl
{
	static constexpr auto FACTION_KEY = "kFaction"sv;
	static constexpr auto PERK_KEY = "kPerk"sv;
	static constexpr auto RANK_KEY = "iRank"sv;
	static constexpr auto COLOR_KEY = "kColor"sv;
	static constexpr auto STRENGTH_KEY = "fStrength"sv;

	static constexpr auto ACTOR_SKIN_TINT_INTENSITY_DIVIDER = 100.0_f32;

	[[nodiscard]] static auto GetSkinTintId(const RE::TESNPC& a_actorBase) -> std::optional<std::uint16_t>
	{
		const auto* actorRace = a_actorBase.formRace;
		if (!actorRace) {
			return std::nullopt;
		}

		const auto actorSex = a_actorBase.GetSex();
		if (actorSex == RE::SEX::kNone) {
			return std::nullopt;
		}

		const auto* actorFaceData = actorRace->faceRelatedData[std::to_underlying(actorSex)];
		if (!actorFaceData) {
			return std::nullopt;
		}

		const auto* actorFaceTintTemplate = actorFaceData->tintingTemplate;
		if (!actorFaceTintTemplate) {
			return std::nullopt;
		}

		for (const auto* actorFaceTintTemplateGroup : actorFaceTintTemplate->groups) {
			if (!actorFaceTintTemplateGroup) {
				continue;
			}

			const auto* actorFaceTintTemplateEntry = actorFaceTintTemplateGroup->GetEntryBySlot(RE::BGSCharacterTint::EntrySlot::kSkinTone);
			if (!actorFaceTintTemplateEntry) {
				continue;
			}

			return actorFaceTintTemplateEntry->uniqueID;
		}

		return std::nullopt;
	}

	[[nodiscard]] static RE::BGSCharacterTint::Entry* GetSkinTintEntry(RE::TESNPC& a_actorBase, std::uint16_t a_skinTintId)
	{
		auto* actorTintingData = a_actorBase.tintingData;
		if (!actorTintingData) {
			return nullptr;
		}

		auto* actorTintingDataEntry = actorTintingData->GetEntryByID(a_skinTintId);
		if (!actorTintingDataEntry) {
			return nullptr;
		}

		return actorTintingDataEntry;
	}
}

namespace Plugin::Papyrus::Forms::ActorBase
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	using ActorBaseFlags = RE::ACTOR_BASE_DATA::Flags;
	using ActorBaseTemplateFlags = RE::ACTOR_BASE_DATA::TEMPLATE_USE_FLAG;
	using ActorBaseRecordFlags = RE::TESNPC::RecordFlags;
	using ActorBaseChangeFlags = RE::TESNPC::ChangeFlags;

	static RE::BGSLocalizedString GetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return {};
		}

		return a_actorBase->shortName;
	}

	static void SetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->shortName = RE::BGSLocalizedString(a_value.GetValue());
	}

	static bool GetIsFemale(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return false;
		}

		return a_actorBase->actorData.actorBaseFlags.any_atomic(RE::ACTOR_BASE_DATA::Flags::kFemale);
	}

	static void SetIsFemale(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase, bool a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->actorData.actorBaseFlags.set_atomic(a_value, RE::ACTOR_BASE_DATA::Flags::kFemale);
		a_actorBase->AddChange(RE::TESNPC::ChangeFlags::kGender);
	}

	static REX::Float32 GetMinHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->minHeight).load(std::memory_order_acquire);
	}

	static void SetMinHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		REX::Float32 a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->minHeight).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetMaxHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->maxHeight).load(std::memory_order_acquire);
	}

	static void SetMaxHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		REX::Float32 a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->maxHeight).store(a_value, std::memory_order_release);
	}

	static RE::BGSVoiceType* GetVoiceType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->voiceType).load(std::memory_order_acquire);
	}

	static void SetVoiceType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSVoiceType* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->voiceType).store(a_value, std::memory_order_release);
	}

	static std::optional<Vector3Struct> GetWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto weight = std::atomic_ref(a_actorBase->morphWeight).load(std::memory_order_acquire);
		return Vector3Struct::FromNiPoint3(weight);
	}

	static void SetWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::optional<Vector3Struct> a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->morphWeight).store(Vector3Struct::ToNiPoint3(a_value.value()), std::memory_order_release);
	}

	static RE::TESLevItem* GetDeathItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->deathItem).load(std::memory_order_acquire);
	}

	static void SetDeathItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESLevItem* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->deathItem).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->actorData.level).load(std::memory_order_acquire);
	}

	static void SetLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::uint16_t a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->actorData.level).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kData);
	}

	static std::uint16_t GetMinLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->actorData.calcLevelMin).load(std::memory_order_acquire);
	}

	static void SetMinLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::uint16_t a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->actorData.calcLevelMin).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kData);
	}

	static std::uint16_t GetMaxLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->actorData.calcLevelMax).load(std::memory_order_acquire);
	}

	static void SetMaxLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::uint16_t a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->actorData.calcLevelMax).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kData);
	}

	static std::int16_t GetXPValueOffset(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->actorData.xpValueOffset).load(std::memory_order_acquire);
	}

	static void SetXPValueOffset(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::int16_t a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->actorData.xpValueOffset).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kData);
	}

	static std::int16_t GetBleedoutOverride(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_actorBase->actorData.bleedoutOverride).load(std::memory_order_acquire);
	}

	static void SetBleedoutOverride(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::int16_t a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->actorData.bleedoutOverride).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kData);
	}

	static RE::TESClass* GetClass(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->cl).load(std::memory_order_acquire);
	}

	static void SetClass(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESClass* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->cl).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kClass);
	}

	static std::optional<std::vector<RE::TESForm*>> GetTemplates(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto templates = a_actorBase->GetTemplates();
		return std::vector<RE::TESForm*>{ templates.begin(), templates.end() };
	}

	static void SetTemplates(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplates(a_values);
	}

	static RE::TESForm* GetTemplateByFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseTemplateFlags a_flag)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(a_flag);
	}

	static void SetTemplateByFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseTemplateFlags a_flag,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(a_flag, a_value);
	}

	static RE::TESForm* GetTraitsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kTraits);
	}

	static void SetTraitsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kTraits, a_value);
	}

	static RE::TESForm* GetStatsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kStats);
	}

	static void SetStatsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kStats, a_value);
	}

	static RE::TESForm* GetFactionsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kFactions);
	}

	static void SetFactionsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kFactions, a_value);
	}

	static RE::TESForm* GetSpellsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kSpells);
	}

	static void SetSpellsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kSpells, a_value);
	}

	static RE::TESForm* GetAIDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kAIData);
	}

	static void SetAIDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kAIData, a_value);
	}

	static RE::TESForm* GetAIPackagesTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kAIPackages);
	}

	static void SetAIPackagesTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kAIPackages, a_value);
	}

	static RE::TESForm* GetBaseDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kBaseData);
	}

	static void SetBaseDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kBaseData, a_value);
	}

	static RE::TESForm* GetInventoryTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kInventory);
	}

	static void SetInventoryTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kInventory, a_value);
	}

	static RE::TESForm* GetScriptTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kScript);
	}

	static void SetScriptTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kScript, a_value);
	}

	static RE::TESForm* GetAIDefaultPackageListTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::BSScript::StaticTag /*a_staticTag*/, RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kAIDefPackList);
	}

	static void SetAIDefaultPackageListTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::BSScript::StaticTag /*a_staticTag*/, RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kAIDefPackList, a_value);
	}

	static RE::TESForm* GetAttackDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kAttackData);
	}

	static void SetAttackDataTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kAttackData, a_value);
	}

	static RE::TESForm* GetKeywordsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetTemplate(ActorBaseTemplateFlags::kKeywords);
	}

	static void SetKeywordsTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetTemplate(ActorBaseTemplateFlags::kKeywords, a_value);
	}

	static RE::TESForm* GetDefaultTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->baseTemplateForm).load(std::memory_order_acquire);
	}

	static void SetDefaultTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->baseTemplateForm).store(a_value, std::memory_order_release);
	}

	static RE::TESForm* GetLegendaryTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->legendaryTemplate).load(std::memory_order_acquire);
	}

	static void SetLegendaryTemplate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->legendaryTemplate).store(a_value, std::memory_order_release);
	}

	static RE::TESGlobal* GetLegendaryChanceGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->legendaryChance).load(std::memory_order_acquire);
	}

	static void SetLegendaryChanceGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESGlobal* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->legendaryChance).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<FactionRank>> GetFactions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& factionList = a_actorBase->factions;

		auto result = std::vector<FactionRank>();
		result.reserve(factionList.size());

		for (const auto& faction : factionList) {
			auto entry = FactionRank::Create();
			entry.Insert(Impl::FACTION_KEY, faction.faction);
			entry.Insert(Impl::RANK_KEY, faction.rank);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetFactions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<FactionRank> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		auto& factionList = a_actorBase->factions;
		factionList.clear();
		factionList.reserve(static_cast<std::uint32_t>(a_values.size()));

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* faction = entry.Find<RE::TESFaction*>(Impl::FACTION_KEY).value();
			auto rank = entry.Find<std::int8_t>(Impl::RANK_KEY).value();

			if (!faction) {
				continue;
			}

			factionList.push_back(
				RE::FACTION_RANK{ .faction = faction, .rank = rank });
		}

		a_actorBase->AddChange(ActorBaseChangeFlags::kFactions);
	}

	static RE::TESFaction* GetCrimeFaction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->crimeFaction).load(std::memory_order_acquire);
	}

	static void SetCrimeFaction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESFaction* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->crimeFaction).store(a_value, std::memory_order_release);
	}

	static RE::TESCombatStyle* GetCombatStyle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->combatStyle).load(std::memory_order_acquire);
	}

	static void SetCombatStyle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESCombatStyle* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->combatStyle).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<RE::TESPackage*>> GetAIPackages(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& packageList = a_actorBase->aiPackList.listPackages;
		return std::vector<RE::TESPackage*>{ packageList.begin(), packageList.end() };
	}

	static void SetAIPackages(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<RE::TESPackage*> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->aiPackList.listPackages = { a_values.begin(), a_values.end() };
	}

	static RE::BGSListForm* GetDefaultPackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->defaultPackList).load(std::memory_order_acquire);
	}

	static void SetDefaultPackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSListForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->defaultPackList).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetSpectatorOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->spectatorOverridePackList).load(std::memory_order_acquire);
	}

	static void SetSpectatorOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSListForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->spectatorOverridePackList).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetObserveCorpseOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->observeCorpseOverridePackList).load(std::memory_order_acquire);
	}

	static void SetObserveCorpseOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSListForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->observeCorpseOverridePackList).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetGuardWarnOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->guardWarnOverridePackList).load(std::memory_order_acquire);
	}

	static void SetGuardWarnOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSListForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->guardWarnOverridePackList).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetCombatOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->enterCombatOverridePackList).load(std::memory_order_acquire);
	}

	static void SetCombatOverridePackageList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSListForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->enterCombatOverridePackList).store(a_value, std::memory_order_release);
	}

	static std::optional<RE::BSTArray<RE::BGSRelationship*>> GetRelationships(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* relationships = a_actorBase->relationships;
		if (!relationships) {
			return std::nullopt;
		}

		return *relationships;
	}

	static void SetRelationships(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::optional<RE::BSTArray<RE::BGSRelationship*>> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		auto& relationships = reinterpret_cast<std::unique_ptr<RE::BSTArray<RE::BGSRelationship*>>&>(a_actorBase->relationships);
		if (!a_values.has_value()) {
			relationships.reset();
			return;
		}

		if (!relationships) {
			relationships = std::make_unique<RE::BSTArray<RE::BGSRelationship*>>();
		}

		relationships->clear();
		*relationships = std::move(a_values).value();
	}

	static RE::BGSOutfit* GetDefaultOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->defaultOutfit).load(std::memory_order_acquire);
	}

	static void SetDefaultOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSOutfit* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->defaultOutfit).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kDefaultOutfit);
	}

	static RE::BGSOutfit* GetSleepOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->sleepOutfit).load(std::memory_order_acquire);
	}

	static void SetSleepOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSOutfit* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->sleepOutfit).store(a_value, std::memory_order_release);
		a_actorBase->AddChange(ActorBaseChangeFlags::kSleepOutfit);
	}

	static RE::TESFurniture* GetPowerArmorFurniture(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_actorBase->powerArmorFurniture).load(std::memory_order_acquire);
	}

	static void SetPowerArmorFurniture(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::TESFurniture* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_actorBase->powerArmorFurniture).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<RE::SpellItem*>> GetSpells(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* spellData = a_actorBase->spellData;
		if (!spellData) {
			return std::nullopt;
		}

		const auto spells = spellData->GetSpells();
		return std::vector<RE::SpellItem*>{ spells.begin(), spells.end() };
	}

	static void SetSpells(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<RE::SpellItem*> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		auto& spellData = reinterpret_cast<std::unique_ptr<RE::TESNPC::SpellData>&>(a_actorBase->spellData);
		if (!spellData) {
			spellData = std::make_unique<RE::TESNPC::SpellData>();
		}

		spellData->SetSpells(a_values);
		a_actorBase->AddChange(ActorBaseChangeFlags::kSpellList);
	}

	static std::optional<std::vector<PerkRank>> GetPerks(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto perkRanks = a_actorBase->GetPerks();

		// BGSPerkRankArray::GetPerks() returns a span built from `perks.data()` and
		// the separately-stored `perkCount`. For some base forms (notably the
		// player's) these desync — `perkCount` is non-zero while the backing array
		// is null/stale — so iterating the span walks invalid PerkRankData* and
		// crashes to desktop. Bail out when the backing storage is missing.
		// (GetFactions is unaffected: it iterates its container by size().)
		if (!perkRanks.data()) {
			return std::vector<PerkRank>{};
		}

		auto result = std::vector<PerkRank>();
		result.reserve(perkRanks.size());

		for (const auto* perkRank : perkRanks) {
			if (!perkRank) {
				continue;
			}

			auto entry = PerkRank::Create();
			entry.Insert(Impl::PERK_KEY, perkRank->perk);
			entry.Insert(Impl::RANK_KEY, perkRank->currentRank);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetPerks(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<PerkRank> a_values)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		auto perkRanks = std::vector<std::unique_ptr<RE::PerkRankData>>();
		perkRanks.reserve(a_values.size());

		for (const auto& entry : a_values) {
			auto* perk = entry.Find<RE::BGSPerk*>(Impl::PERK_KEY).value();
			if (!perk) {
				continue;
			}

			const auto rank = entry.Find<std::uint8_t>(Impl::RANK_KEY).value();

			perkRanks.push_back(std::make_unique<RE::PerkRankData>(perk, rank));
		}

		a_actorBase->SetPerks(perkRanks);
	}

	static RE::BGSColorForm* GetHairColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetHairColor();
	}

	static void SetHairColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSColorForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetHairColor(a_value);
	}

	static RE::BGSColorForm* GetFacialHairColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetFacialHairColor();
	}

	static void SetFacialHairColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSColorForm* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetFacialHairColor(a_value);
	}

	static RE::BGSTextureSet* GetFaceDetails(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetFaceDetails();
	}

	static void SetFaceDetails(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		RE::BGSTextureSet* a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetFaceDetails(a_value);
	}

	static SkinTintData GetSkinTintData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		const auto skinTintId = Impl::GetSkinTintId(*a_actorBase);
		if (!skinTintId.has_value()) {
			return nullptr;
		}

		const auto* skinTintEntry = Impl::GetSkinTintEntry(*a_actorBase, skinTintId.value());
		if (!skinTintEntry) {
			return nullptr;
		}

		const auto* skinTintPaletteEntry = RE::DynamicCast<const RE::BGSCharacterTint::PaletteEntry*>(skinTintEntry);
		if (!skinTintPaletteEntry) {
			return nullptr;
		}

		auto skinTintData = SkinTintData::Create();
		skinTintData.Insert(Impl::COLOR_KEY, ColorStruct::FromHexBgr(skinTintPaletteEntry->tintingColor));
		skinTintData.Insert(Impl::STRENGTH_KEY, static_cast<REX::Float32>(skinTintPaletteEntry->tintingValue) / Impl::ACTOR_SKIN_TINT_INTENSITY_DIVIDER);

		return skinTintData;
	}

	static bool SetSkinTintData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		SkinTintData a_data)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return false;
		}

		if (!a_data) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto skinTintId = Impl::GetSkinTintId(*a_actorBase);
		if (!skinTintId.has_value()) {
			return false;
		}

		const auto* skinTintEntry = Impl::GetSkinTintEntry(*a_actorBase, skinTintId.value());
		const auto skinTintEntryStrength = skinTintEntry ? skinTintEntry->tintingValue : 0;

		const auto* skinTintPaletteEntry = RE::DynamicCast<const RE::BGSCharacterTint::PaletteEntry*>(skinTintEntry);
		const auto skinTintPaletteEntryColor = skinTintPaletteEntry ? skinTintPaletteEntry->tintingColor : 0;

		const auto skinTintColor = a_data.Find<std::optional<ColorStruct>>(Impl::COLOR_KEY).value();
		const auto skinTintStrength = a_data.Find<REX::Float32>(Impl::STRENGTH_KEY).value();

		const auto newSkinTintColor = skinTintColor.has_value() ? skinTintColor->ToHexBgr() : skinTintPaletteEntryColor;
		const auto newSkinTintStrength = skinTintStrength > 0.0_f32 ? skinTintStrength : (static_cast<REX::Float32>(skinTintEntryStrength) / Impl::ACTOR_SKIN_TINT_INTENSITY_DIVIDER);

		a_actorBase->SetTintingData(skinTintId.value(), newSkinTintStrength, newSkinTintColor);
		a_actorBase->AddChange(RE::TESNPC::ChangeFlags::kFace);

		if (!a_actorBase->IsPlayer()) {
			return true;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		player->SetTintingData(skinTintId.value(), newSkinTintStrength, newSkinTintColor);
		return true;
	}

	static std::optional<std::vector<RE::BGSHeadPart*>> GetHeadParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto headParts = a_actorBase->GetHeadParts();
		return std::vector<RE::BGSHeadPart*>{ headParts.begin(), headParts.end() };
	}

	static void SetHeadParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		std::vector<RE::BGSHeadPart*> a_value)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->SetHeadParts(a_value);
	}

	static RE::TESNPC* GetRootFaceActorBase(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return nullptr;
		}

		return a_actorBase->GetRootFaceNPC();
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseFlags a_flag)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return false;
		}

		return a_actorBase->actorData.actorBaseFlags.any_atomic(a_flag);
	}

	static ActorBaseFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return ActorBaseFlags::kNone;
		}

		return a_actorBase->actorData.actorBaseFlags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseFlags a_flags)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->actorData.actorBaseFlags.set_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseFlags a_flag,
		bool a_set)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->actorData.actorBaseFlags.set_atomic(a_set, a_flag);
	}

	static bool HasTemplateFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseTemplateFlags a_flag)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return false;
		}

		return a_actorBase->actorData.templateUseFlags.any_atomic(a_flag);
	}

	static ActorBaseTemplateFlags GetTemplateFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return ActorBaseTemplateFlags::kNone;
		}

		return a_actorBase->actorData.templateUseFlags.get_atomic();
	}

	static void SetTemplateFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseTemplateFlags a_flags)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->actorData.templateUseFlags.set_atomic(a_flags);
	}

	static void SetTemplateFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESNPC* a_actorBase,
		ActorBaseTemplateFlags a_flag,
		bool a_set)
	{
		if (!a_actorBase) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_BASE_NULL, a_stackId);
			return;
		}

		a_actorBase->actorData.templateUseFlags.set_atomic(a_set, a_flag);
	}

	static ActorBaseFlags GetFlag_IsFemale(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kFemale; }
	static ActorBaseFlags GetFlag_IsEssential(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kEssential; }
	static ActorBaseFlags GetFlag_IsCharGenFacePreset(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kIsChargenFacePreset; }
	static ActorBaseFlags GetFlag_Respawn(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kRespawn; }
	static ActorBaseFlags GetFlag_AutoCalcStats(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kAutoCalcStats; }
	static ActorBaseFlags GetFlag_IsUnique(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kUnique; }
	static ActorBaseFlags GetFlag_DoNotAffectStealthMeter(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kDoesNotAffectStealthMeter; }
	static ActorBaseFlags GetFlag_HasPlayerLevelMult(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kPCLevelMult; }
	static ActorBaseFlags GetFlag_UseTemplates(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kUsesTemplate; }
	static ActorBaseFlags GetFlag_IsProtected(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kProtected; }
	static ActorBaseFlags GetFlag_IsSummonable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kSummonable; }
	static ActorBaseFlags GetFlag_DoNotBleed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kDoesNotBleed; }
	static ActorBaseFlags GetFlag_HasBleedoutOverride(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kBleedoutOverride; }
	static ActorBaseFlags GetFlag_UseOppositeGenderAnims(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kOppositeGenderAnims; }
	static ActorBaseFlags GetFlag_IsSimpleActor(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kSimpleActor; }
	static ActorBaseFlags GetFlag_HasLoopedScript(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kLoopedScript; }
	static ActorBaseFlags GetFlag_HasLoopedAudio(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kLoopedAudio; }
	static ActorBaseFlags GetFlag_IsGhost(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kIsGhost; }
	static ActorBaseFlags GetFlag_IsInvulnerable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseFlags::kInvulnerable; }

	static ActorBaseTemplateFlags GetTemplateFlag_UseTraits(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kTraits; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseStats(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kStats; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseFactions(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kFactions; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseSpells(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kSpells; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseAIData(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kAIData; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseAIPackages(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kAIPackages; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseBaseData(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kBaseData; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseInventory(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kInventory; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseScript(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kScript; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseAIDefaultPackageList(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kAIDefPackList; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseAttackData(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kAttackData; }
	static ActorBaseTemplateFlags GetTemplateFlag_UseKeywords(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseTemplateFlags::kKeywords; }

	static ActorBaseRecordFlags GetRecordFlag_IsCompressed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseRecordFlags::kCompressed; }
	static ActorBaseRecordFlags GetRecordFlag_HasBleedoutOverride(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActorBaseRecordFlags::kBleedoutOverride; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetShortName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetShortName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsFemale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsFemale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMinHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMinHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaxHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaxHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVoiceType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVoiceType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeathItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDeathItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMinLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMinLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaxLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaxLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetXPValueOffset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetXPValueOffset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBleedoutOverride);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBleedoutOverride);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetClass);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetClass);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetTemplates);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetTemplates);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetTemplateByFlag);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetTemplateByFlag);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetTraitsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetTraitsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetStatsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetStatsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFactionsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFactionsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSpellsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSpellsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAIDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAIDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAIPackagesTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAIPackagesTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetBaseDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetBaseDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetInventoryTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetInventoryTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetScriptTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAIDefaultPackageListTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAIDefaultPackageListTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAttackDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAttackDataTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeywordsTemplate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetKeywordsTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDefaultTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLegendaryTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLegendaryTemplate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLegendaryChanceGlobal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLegendaryChanceGlobal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFactions);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFactions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCrimeFaction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCrimeFaction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatStyle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCombatStyle);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAIPackages);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAIPackages);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultPackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDefaultPackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpectatorOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpectatorOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetObserveCorpseOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetObserveCorpseOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetGuardWarnOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetGuardWarnOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatOverridePackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCombatOverridePackageList);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRelationships);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRelationships);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDefaultOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSleepOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSleepOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPowerArmorFurniture);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPowerArmorFurniture);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSpells);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSpells);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPerks);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPerks);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetHairColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetHairColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFacialHairColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFacialHairColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFaceDetails);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFaceDetails);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSkinTintData);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSkinTintData);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetHeadParts);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetHeadParts);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRootFaceActorBase);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasTemplateFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTemplateFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTemplateFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsFemale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsEssential);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsCharGenFacePreset);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_Respawn);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AutoCalcStats);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DoNotAffectStealthMeter);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsUnique);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasPlayerLevelMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseTemplates);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsProtected);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSummonable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DoNotBleed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasBleedoutOverride);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseOppositeGenderAnims);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSimpleActor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasLoopedScript);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasLoopedAudio);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsGhost);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsInvulnerable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseTraits);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseStats);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseFactions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseSpells);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseAIData);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseAIPackages);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseBaseData);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseInventory);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseScript);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseAIDefaultPackageList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseAttackData);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTemplateFlag_UseKeywords);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsCompressed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasBleedoutOverride);
	}
}
