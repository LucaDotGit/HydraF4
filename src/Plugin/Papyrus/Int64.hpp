#pragma once

namespace Plugin::Papyrus::Int64
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Int64");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
