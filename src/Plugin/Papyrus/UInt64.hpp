#pragma once

namespace Plugin::Papyrus::UInt64
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:UInt64");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
