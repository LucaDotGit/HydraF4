#pragma once

namespace Plugin::Papyrus::Timer
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Timer");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
