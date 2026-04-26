#pragma once

namespace Plugin::Papyrus::Regex
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Regex");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
