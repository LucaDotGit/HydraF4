#pragma once

namespace Plugin::Papyrus::StrictStrings
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:StrictStrings");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
