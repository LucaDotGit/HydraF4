#pragma once

namespace Plugin::Papyrus::Char
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Char");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
