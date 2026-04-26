#pragma once

namespace Plugin::Papyrus::UInt32
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:UInt32");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
