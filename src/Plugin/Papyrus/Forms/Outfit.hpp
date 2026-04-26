#pragma once

namespace Plugin::Papyrus::Forms::Outfit
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Outfit");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
