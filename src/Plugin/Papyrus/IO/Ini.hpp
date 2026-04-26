#pragma once

namespace Plugin::Papyrus::IO::Ini
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Ini");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
