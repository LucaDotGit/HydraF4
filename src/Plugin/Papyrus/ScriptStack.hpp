#pragma once

namespace Plugin::Papyrus::ScriptStack
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:ScriptStack");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
