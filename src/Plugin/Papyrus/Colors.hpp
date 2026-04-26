#pragma once

namespace Plugin::Papyrus::Colors
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Colors");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
