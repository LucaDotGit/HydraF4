#include "Plugin/Papyrus/Forms/Climate.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Climate::Impl
{
	static constexpr auto WEATHER_TYPE_KEY = "kWeatherType"sv;
	static constexpr auto CHANCE_KEY = "fChance"sv;
	static constexpr auto CHANCE_GLOBAL_KEY = "kChanceGlobal"sv;
}

namespace Plugin::Papyrus::Forms::Climate
{
	static std::uint8_t GetSunriseBegin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_climate->timing.sunrise.begin).load(std::memory_order_acquire);
	}

	static void SetSunriseBegin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::uint8_t a_value)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_climate->timing.sunrise.begin).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetSunriseEnd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_climate->timing.sunrise.end).load(std::memory_order_acquire);
	}

	static void SetSunriseEnd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::uint8_t a_value)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_climate->timing.sunrise.end).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetSunsetBegin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_climate->timing.sunset.begin).load(std::memory_order_acquire);
	}

	static void SetSunsetBegin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::uint8_t a_value)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_climate->timing.sunset.begin).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetSunsetEnd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_climate->timing.sunset.end).load(std::memory_order_acquire);
	}

	static void SetSunsetEnd(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::uint8_t a_value)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_climate->timing.sunset.end).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetVolatility(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_climate->timing.volatility).load(std::memory_order_acquire);
	}

	static void SetVolatility(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::uint8_t a_value)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_climate->timing.volatility).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<WeatherEntry>> GetWeathers(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& weatherList = a_climate->weatherList;

		auto result = std::vector<WeatherEntry>();
		for (const auto* element : weatherList) {
			auto entry = WeatherEntry::Create();
			entry.Insert(Impl::WEATHER_TYPE_KEY, element->weather);
			entry.Insert(Impl::CHANCE_KEY, element->chance);
			entry.Insert(Impl::CHANCE_GLOBAL_KEY, element->chanceVar);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetWeathers(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESClimate* a_climate,
		std::vector<WeatherEntry> a_values)
	{
		if (!a_climate) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CLIMATE_NULL, a_stackId);
			return;
		}

		auto weatherEntries = std::vector<std::unique_ptr<RE::WeatherEntry>>();
		weatherEntries.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* weather = entry.Find<RE::TESWeather*>(Impl::WEATHER_TYPE_KEY).value();
			auto chance = entry.Find<std::uint32_t>(Impl::CHANCE_KEY).value();
			auto* globalChance = entry.Find<RE::TESGlobal*>(Impl::CHANCE_GLOBAL_KEY).value();

			weatherEntries.push_back(std::make_unique<RE::WeatherEntry>(weather, chance, globalChance));
		}

		a_climate->SetWeatherEntries(weatherEntries);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSunriseBegin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSunriseBegin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSunriseEnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSunriseEnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSunsetBegin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSunsetBegin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSunsetEnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSunsetEnd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVolatility);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVolatility);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetWeathers);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetWeathers);
	}
}
