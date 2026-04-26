#pragma once

namespace Plugin::Papyrus::Forms::ActiveMagicEffect
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:ActiveMagicEffect");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
