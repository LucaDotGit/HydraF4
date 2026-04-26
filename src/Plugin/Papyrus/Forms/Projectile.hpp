#pragma once

namespace Plugin::Papyrus::Forms::Projectile
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Projectile");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
