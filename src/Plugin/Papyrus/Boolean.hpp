#pragma once

namespace Plugin::Papyrus::Boolean
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Boolean");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
