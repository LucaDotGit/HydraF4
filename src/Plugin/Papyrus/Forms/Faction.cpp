#include "Plugin/Papyrus/Forms/Faction.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Faction::Impl
{
	static constexpr auto FACTION_KEY = "kFaction"sv;
	static constexpr auto MODIFIER_KEY = "iModifier"sv;
	static constexpr auto COMBAT_REACTION_KEY = "iCombatReaction"sv;
}

namespace Plugin::Papyrus::Forms::Faction
{
	using FactionFlags = RE::FACTION_DATA::Flags;
	using FactionChangeFlags = RE::TESFaction::ChangeFlags;

	static RE::TESObjectREFR* GetExteriorJailMarker(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.factionJailMarker).load(std::memory_order_acquire);
	}

	static void SetExteriorJailMarker(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::TESObjectREFR* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.factionJailMarker).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectREFR* GetCompanionWaitMarker(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.factionWaitMarker).load(std::memory_order_acquire);
	}

	static void SetCompanionWaitMarker(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::TESObjectREFR* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.factionWaitMarker).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectREFR* GetStolenItemsContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.factionStolenContainer).load(std::memory_order_acquire);
	}

	static void SetStolenItemsContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::TESObjectREFR* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.factionStolenContainer).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectREFR* GetPlayerInventoryContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.factionPlayerInventoryContainer).load(std::memory_order_acquire);
	}

	static void SetPlayerInventoryContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::TESObjectREFR* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.factionPlayerInventoryContainer).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetSharedCrimeFactions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.crimeGroup).load(std::memory_order_acquire);
	}

	static void SetSharedCrimeFactions(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::BGSListForm* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeGroup).store(a_value, std::memory_order_release);
	}

	static RE::BGSOutfit* GetJailOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->crimeData.jailOutfit).load(std::memory_order_acquire);
	}

	static void SetJailOutfit(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::BGSOutfit* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.jailOutfit = a_value).store(a_value, std::memory_order_release);
	}

	static bool GetDoesArrest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.arrest).load(std::memory_order_acquire);
	}

	static void SetDoesArrest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		bool a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.arrest).store(a_value, std::memory_order_release);
	}

	static bool GetDoesAttackOnSight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.attackOnSight).load(std::memory_order_acquire);
	}

	static void SetDoesAttackOnSight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		bool a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.attackOnSight).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetMurderValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.murderCrimeGold).load(std::memory_order_acquire);
	}

	static void SetMurderValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.murderCrimeGold).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetAssaultValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.assaultCrimeGold).load(std::memory_order_acquire);
	}

	static void SetAssaultValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.assaultCrimeGold).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetTrespassValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.trespassCrimeGold).load(std::memory_order_acquire);
	}

	static void SetTrespassValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.trespassCrimeGold).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetPickpocketValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.pickpocketCrimeGold).load(std::memory_order_acquire);
	}

	static void SetPickpocketValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.pickpocketCrimeGold).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetEscapeValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.escapeCrimeGold).load(std::memory_order_acquire);
	}

	static void SetEscapeValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.escapeCrimeGold).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetStealMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_faction->crimeData.crimeValues.stealCrimeGoldMult).load(std::memory_order_acquire);
	}

	static void SetStealMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		REX::Float32 a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->crimeData.crimeValues.stealCrimeGoldMult).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetMerchantTradeList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->vendorData.vendorSellBuyList).load(std::memory_order_acquire);
	}

	static void SetMerchantTradeList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::BGSListForm* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorSellBuyList).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static RE::TESObjectREFR* GetMerchantContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_faction->vendorData.merchantContainer).load(std::memory_order_acquire);
	}

	static void SetMerchantContainer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		RE::TESObjectREFR* a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.merchantContainer).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static std::uint16_t GetMerchantStartHour(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.startHour).load(std::memory_order_acquire);
	}

	static void SetMerchantStartHour(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.startHour).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static std::uint16_t GetMerchantEndHour(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.endHour).load(std::memory_order_acquire);
	}

	static void SetMerchantEndHour(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint16_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.endHour).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static std::uint32_t GetMerchantRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.locationRadius).load(std::memory_order_acquire);
	}

	static void SetMerchantRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::uint32_t a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.locationRadius).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static bool GetBuysStolenItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.buysStolen).load(std::memory_order_acquire);
	}

	static void SetBuysStolenItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		bool a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.buysStolen).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static bool GetBuysNonStolenItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.buysNonStolen).load(std::memory_order_acquire);
	}

	static void SetBuysNonStolenItems(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		bool a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.buysNonStolen).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static bool GetTradesEverything(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_faction->vendorData.vendorValues.notBuySell).load(std::memory_order_acquire);
	}

	static void SetTradesEverything(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		bool a_value)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_faction->vendorData.vendorValues.notBuySell).store(a_value, std::memory_order_release);
		a_faction->AddChange(FactionChangeFlags::kVendorData);
	}

	static std::optional<std::vector<RelationEntry>> GetRelations(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& factionReactions = a_faction->reactionList;

		auto result = std::vector<RelationEntry>();

		for (const auto& reaction : factionReactions) {
			if (!reaction) {
				continue;
			}

			auto entry = RelationEntry::Create();
			entry.Insert(Impl::FACTION_KEY, reaction->form);
			entry.Insert(Impl::MODIFIER_KEY, reaction->reaction);
			entry.Insert(Impl::COMBAT_REACTION_KEY, reaction->fightReaction.get());

			result.push_back(std::move(entry));
		};

		return result;
	}

	static void SetRelations(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		std::vector<RelationEntry> a_values)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		auto reactions = std::vector<std::unique_ptr<RE::GROUP_REACTION>>();
		reactions.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* faction = entry.Find<RE::TESFaction*>(Impl::FACTION_KEY).value();
			auto modifier = entry.Find<std::int32_t>(Impl::MODIFIER_KEY).value();
			auto combatReaction = entry.Find<RE::FIGHT_REACTION>(Impl::COMBAT_REACTION_KEY).value();

			reactions.push_back(std::make_unique<RE::GROUP_REACTION>(faction, modifier, combatReaction));
		}

		a_faction->SetReactions(reactions);
	}

	static RE::FIGHT_REACTION GetCombatReaction_Neutral(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::FIGHT_REACTION::kNeutral; }
	static RE::FIGHT_REACTION GetCombatReaction_Enemy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::FIGHT_REACTION::kEnemy; }
	static RE::FIGHT_REACTION GetCombatReaction_Ally(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::FIGHT_REACTION::kAlly; }
	static RE::FIGHT_REACTION GetCombatReaction_Friend(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::FIGHT_REACTION::kFriend; }

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		FactionFlags a_flag)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return false;
		}

		return a_faction->data.flags.any_atomic(a_flag);
	}

	static FactionFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return FactionFlags::kNone;
		}

		return a_faction->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		FactionFlags a_flags)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		a_faction->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFaction* a_faction,
		FactionFlags a_flag,
		bool a_set)
	{
		if (!a_faction) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FACTION_NULL, a_stackId);
			return;
		}

		a_faction->data.flags.set_atomic(a_set, a_flag);
	}

	static FactionFlags GetFlag_IsHiddenFromPlayer(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kHiddenFromPC; }
	static FactionFlags GetFlag_IsSpecialInCombat(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kSpecialCombat; }
	static FactionFlags GetFlag_IsPlayerExpelled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kPlayerIsExpelled; }
	static FactionFlags GetFlag_IsPlayerEnemy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kPlayerIsEnemy; }
	static FactionFlags GetFlag_TrackCrime(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kTrackCrime; }
	static FactionFlags GetFlag_IgnoreMurder(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kIgnoresCrimes_Murder; }
	static FactionFlags GetFlag_IgnoreAssault(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kIgnoresCrimes_Assault; }
	static FactionFlags GetFlag_IgnoreStealing(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kIgnoresCrimes_Stealing; }
	static FactionFlags GetFlag_IgnoreTrespass(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kIgnoresCrimes_Trespass; }
	static FactionFlags GetFlag_DoNotReportMemberCrime(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kDoNotReportCrimesAgainstMembers; }
	static FactionFlags GetFlag_UseDefaultCrimeValue(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kCrimeGold_UseDefaults; }
	static FactionFlags GetFlag_IgnorePickpocketing(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kIgnoresCrimes_Pickpocket; }
	static FactionFlags GetFlag_IsMerchant(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kVendor; }
	static FactionFlags GetFlag_CanBeOwner(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FactionFlags::kCanBeOwner; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExteriorJailMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetExteriorJailMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCompanionWaitMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCompanionWaitMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetStolenItemsContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetStolenItemsContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPlayerInventoryContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPlayerInventoryContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSharedCrimeFactions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSharedCrimeFactions);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetJailOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetJailOutfit);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDoesArrest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDoesArrest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDoesAttackOnSight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDoesAttackOnSight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMurderValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMurderValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAssaultValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAssaultValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTrespassValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTrespassValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPickpocketValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPickpocketValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEscapeValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEscapeValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetStealMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetStealMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMerchantTradeList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMerchantTradeList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMerchantContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMerchantContainer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMerchantStartHour);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMerchantStartHour);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMerchantEndHour);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMerchantEndHour);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMerchantRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMerchantRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBuysStolenItems);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBuysStolenItems);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBuysNonStolenItems);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBuysNonStolenItems);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTradesEverything);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTradesEverything);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRelations);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRelations);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatReaction_Neutral);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatReaction_Enemy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatReaction_Ally);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCombatReaction_Friend);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHiddenFromPlayer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSpecialInCombat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPlayerExpelled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPlayerEnemy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_TrackCrime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreMurder);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreAssault);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreStealing);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreTrespass);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DoNotReportMemberCrime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseDefaultCrimeValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnorePickpocketing);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsMerchant);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanBeOwner);
	}
}