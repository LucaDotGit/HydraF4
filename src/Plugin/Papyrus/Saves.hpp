#pragma once

namespace Plugin::Papyrus::Saves
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Saves");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
