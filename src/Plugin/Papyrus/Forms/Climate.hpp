#pragma once

namespace Plugin::Papyrus::Forms::Climate
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Climate");

	using WeatherEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "WeatherEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
