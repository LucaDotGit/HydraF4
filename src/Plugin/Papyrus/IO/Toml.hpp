#pragma once

namespace Plugin::Papyrus::IO::Toml
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Toml");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
