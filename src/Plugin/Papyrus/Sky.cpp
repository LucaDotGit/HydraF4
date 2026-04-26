#include "Plugin/Papyrus/Sky.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Sky
{
	using SkyFlags = RE::Sky::Flags;

	static RE::TESRegion* GetCurrentRegion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->currentRegion).load(std::memory_order_acquire);
	}

	static RE::TESClimate* GetCurrentClimate(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->currentClimate).load(std::memory_order_acquire);
	}

	static void SetCurrentClimate(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		sky->currentClimate = a_climate;
		sky->flags.set(
			SkyFlags::kUpdateSunriseBegin,
			SkyFlags::kUpdateSunriseEnd,
			SkyFlags::kUpdateSunsetBegin,
			SkyFlags::kUpdateSunsetEnd);
	}

	static RE::TESWeather* GetDefaultWeather(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->defaultWeather).load(std::memory_order_acquire);
	}

	static RE::TESWeather* GetLastWeather(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->lastWeather).load(std::memory_order_acquire);
	}

	static RE::TESWeather* GetCurrentWeather(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->currentWeather).load(std::memory_order_acquire);
	}

	static RE::TESWeather* GetWeatherOverride(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return std::atomic_ref(sky->overrideWeather).load(std::memory_order_acquire);
	}

	static bool RemoveWeatherOverride(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto* sky = RE::Sky::GetSingleton();
		if (!sky) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto* previousWeather = std::exchange(sky->overrideWeather, nullptr);
		sky->flags.set(previousWeather != nullptr, RE::Sky::Flags::kReleaseWeatherOverride);
		return previousWeather != nullptr;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentRegion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentClimate);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetCurrentClimate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultWeather);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLastWeather);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentWeather);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWeatherOverride);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveWeatherOverride);
	}
}
