#pragma once

namespace Plugin::Papyrus::Versions
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Versions");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
