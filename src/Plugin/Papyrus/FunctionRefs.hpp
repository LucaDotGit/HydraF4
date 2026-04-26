#pragma once

namespace Plugin::Papyrus::FunctionRefs
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:FunctionRefs");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
