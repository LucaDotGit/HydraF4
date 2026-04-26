#pragma once

namespace Plugin::Papyrus::Random
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Random");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
