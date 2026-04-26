#pragma once

namespace Plugin::Papyrus::Internal
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Internal");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
