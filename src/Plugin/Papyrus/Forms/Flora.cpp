#include "Plugin/Papyrus/Forms/Flora.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Flora
{
	using Season = RE::TESProduceForm::Season;

	static RE::TESBoundObject* GetHarvestItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_flora->produceItem).load(std::memory_order_acquire);
	}

	static void SetHarvestItem(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		RE::TESBoundObject* a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->produceItem).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetHarvestSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_flora->harvestSound).load(std::memory_order_acquire);
	}

	static void SetHarvestSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->harvestSound).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetSpringHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kSpring)]).load(std::memory_order_acquire);
	}

	static void SetSpringHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		std::uint8_t a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kSpring)]).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetSummerHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kSummer)]).load(std::memory_order_acquire);
	}

	static void SetSummerHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		std::uint8_t a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kSummer)]).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetFallHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kFall)]).load(std::memory_order_acquire);
	}

	static void SetFallHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		std::uint8_t a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kFall)]).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetWinterHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kWinter)]).load(std::memory_order_acquire);
	}

	static void SetWinterHarvestChance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFlora* a_flora,
		std::uint8_t a_value)
	{
		if (!a_flora) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FLORA_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_flora->produceChance[std::to_underlying(Season::kWinter)]).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHarvestItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHarvestItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHarvestSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHarvestSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpringHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpringHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSummerHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSummerHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFallHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFallHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWinterHarvestChance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWinterHarvestChance);
	}
}