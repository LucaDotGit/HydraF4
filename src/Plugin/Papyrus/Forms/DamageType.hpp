#pragma once

namespace Plugin::Papyrus::Forms::DamageType
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:DamageType");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
