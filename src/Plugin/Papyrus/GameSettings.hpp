#pragma once

namespace Plugin::Papyrus::GameSettings
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:GameSettings");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
