#pragma once

namespace Plugin::Papyrus::Forms::Enchantment
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Enchantment");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
