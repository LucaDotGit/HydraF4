#pragma once

namespace Plugin::Papyrus::IniSettings
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IniSettings");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
