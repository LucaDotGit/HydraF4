#pragma once

namespace Plugin::Papyrus::MemSet
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:MemSet");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
