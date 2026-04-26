#pragma once

namespace Plugin::Papyrus::Forms::Spell
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Spell");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
