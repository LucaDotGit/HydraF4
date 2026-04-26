#include "Plugin/Papyrus/MiscStats.hpp"

namespace Plugin::Papyrus::MiscStats::Impl
{
	[[nodiscard]] __forceinline static RE::MiscStatManager::MiscStat* GetMiscStatByKey(const RE::BSFixedString& a_key)
	{
		auto& miscStats = RE::MiscStatManager::GetMiscStats();
		return std::ranges::find_if(miscStats, [&a_key](const RE::MiscStatManager::MiscStat& stat) {
			return stat.id == a_key;
		});
	}

	static void OnStatChange(const RE::BSFixedString& a_statId, std::int32_t a_newValue)
	{
		auto miscStatChangeEvent = RE::TESTrackedStatsEvent{
			.statID = a_statId,
			.newValue = a_newValue
		};

		F4SE::GetTaskInterface()->AddTask([miscStatChangeEvent = std::move(miscStatChangeEvent)]() {
			RE::TESTrackedStatsEvent::GetEventSource()->Notify(miscStatChangeEvent);
		});
	}
}

namespace Plugin::Papyrus::MiscStats
{
	static std::vector<RE::BSFixedString> GetKeys(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto& miscStats = RE::MiscStatManager::GetMiscStats();

		auto keys = std::vector<RE::BSFixedString>();
		keys.reserve(miscStats.size());

		for (const auto& stat : miscStats) {
			keys.push_back(stat.id);
		}

		return keys;
	}

	static bool Contains(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		return Impl::GetMiscStatByKey(a_key) != nullptr;
	}

	static RE::BSFixedString GetName(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return {};
		}

		return stat->displayName;
	}

	static RE::MiscStatManager::MiscStatType GetType(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return RE::MiscStatManager::MiscStatType::kGeneral;
		}

		return stat->type.get_atomic();
	}

	static bool GetShowIfZero(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return false;
		}

		return stat->showIfZero;
	}

	static std::int32_t GetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		std::int32_t a_default)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return a_default;
		}

		return stat->value;
	}

	static bool SetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		std::int32_t a_value)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return false;
		}

		stat->value = a_value;
		Impl::OnStatChange(a_key, a_value);

		return true;
	}

	static bool ModValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		std::int32_t a_value)
	{
		auto* stat = Impl::GetMiscStatByKey(a_key);
		if (!stat) {
			return false;
		}

		const auto newValue = stat->value + a_value;
		stat->value = newValue;
		Impl::OnStatChange(a_key, newValue);

		return true;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeys);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetShowIfZero);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModValue);
	}
}
