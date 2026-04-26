#pragma once

namespace Plugin::Papyrus::Arrays
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Arrays");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
