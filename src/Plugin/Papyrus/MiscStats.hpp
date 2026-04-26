#pragma once

namespace Plugin::Papyrus::MiscStats
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:MiscStats");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
