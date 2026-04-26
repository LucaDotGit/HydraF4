#pragma once

namespace Plugin::Papyrus::Int32
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Int32");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
