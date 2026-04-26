#pragma once

namespace Plugin::Papyrus::Float64
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Float64");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
