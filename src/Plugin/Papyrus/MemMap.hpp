#pragma once

namespace Plugin::Papyrus::MemMap
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:MemMap");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
