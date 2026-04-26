#pragma once

namespace Plugin::Papyrus::Pairs
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Pairs");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
