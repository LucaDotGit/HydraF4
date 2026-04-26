#pragma once

namespace Plugin::Papyrus::Forms::Explosion
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Explosion");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
