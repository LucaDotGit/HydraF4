#pragma once

namespace Plugin::Papyrus::Strings
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Strings");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
