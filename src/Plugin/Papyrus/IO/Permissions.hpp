#pragma once

namespace Plugin::Papyrus::IO::Permissions
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Permissions");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
