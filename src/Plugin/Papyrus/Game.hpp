#pragma once

namespace Plugin::Papyrus::Game
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Game");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
