#pragma once

namespace Plugin::Papyrus::StringPairs
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:StringPairs");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
