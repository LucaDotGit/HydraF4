#pragma once

namespace Plugin::Papyrus::Input
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Input");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
