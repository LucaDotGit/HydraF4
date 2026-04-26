#pragma once

namespace Plugin::Papyrus::Hashing
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Hashing");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
