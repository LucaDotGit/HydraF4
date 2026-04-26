#pragma once

namespace Plugin::Papyrus::Vectors2
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Vectors2");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
