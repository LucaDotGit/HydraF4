#pragma once

namespace Plugin::Papyrus::UI
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:UI");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
