#pragma once

namespace Plugin::Papyrus::Sky
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Sky");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
