#pragma once

namespace Plugin::Papyrus::Console
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Console");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
