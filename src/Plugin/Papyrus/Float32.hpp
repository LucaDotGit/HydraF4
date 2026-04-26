#pragma once

namespace Plugin::Papyrus::Float32
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Float32");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
