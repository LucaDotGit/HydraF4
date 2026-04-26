#pragma once

namespace Plugin::Papyrus::TempMap
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:TempMap");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
