#pragma once

namespace Plugin::Papyrus::Forms::Idle
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Idle");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
