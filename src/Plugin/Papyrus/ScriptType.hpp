#pragma once

namespace Plugin::Papyrus::ScriptType
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:ScriptType");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
