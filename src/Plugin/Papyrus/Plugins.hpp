#pragma once

namespace Plugin::Papyrus::Plugins
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Plugins");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
