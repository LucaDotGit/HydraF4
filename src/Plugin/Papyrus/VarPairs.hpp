#pragma once

namespace Plugin::Papyrus::VarPairs
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:VarPairs");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
