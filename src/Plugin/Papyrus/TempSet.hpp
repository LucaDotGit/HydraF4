#pragma once

namespace Plugin::Papyrus::TempSet
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:TempSet");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
