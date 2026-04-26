#pragma once

namespace Plugin::Papyrus::Forms::Potion
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Potion");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
