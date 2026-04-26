#pragma once

namespace Plugin::Papyrus::Wildcards
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Wildcards");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
