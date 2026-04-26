#pragma once

namespace Plugin::Papyrus::Operator
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Operator");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
