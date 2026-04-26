#pragma once

namespace Plugin::Papyrus::Assert
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Assert");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
