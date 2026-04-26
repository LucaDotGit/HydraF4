#pragma once

namespace Plugin::Papyrus::Forms::Ammo
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Ammo");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
