#pragma once

namespace Plugin::Papyrus::Vectors3
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Vectors3");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
