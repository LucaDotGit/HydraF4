#pragma once

namespace Plugin::Papyrus::Mutex
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Mutex");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
