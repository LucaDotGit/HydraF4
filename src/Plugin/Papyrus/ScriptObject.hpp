#pragma once

namespace Plugin::Papyrus::ScriptObject
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:ScriptObject");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
