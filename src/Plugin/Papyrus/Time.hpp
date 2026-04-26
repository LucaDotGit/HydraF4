#pragma once

namespace Plugin::Papyrus::Time
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Time");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
