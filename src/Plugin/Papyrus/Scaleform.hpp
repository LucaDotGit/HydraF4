#pragma once

namespace Plugin::Papyrus::Scaleform
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Scaleform");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
