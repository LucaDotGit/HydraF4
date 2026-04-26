#pragma once

namespace Plugin::Papyrus::Forms::GlobalVariable
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:GlobalVariable");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
