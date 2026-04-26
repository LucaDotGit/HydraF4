#pragma once

namespace Plugin::Papyrus::Forms::Activator
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Activator");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
