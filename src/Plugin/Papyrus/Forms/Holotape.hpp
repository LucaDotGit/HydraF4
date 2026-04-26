#pragma once

namespace Plugin::Papyrus::Forms::Holotape
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Holotape");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
