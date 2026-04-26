#pragma once

namespace Plugin::Papyrus::Vectors4
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Vectors4");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
