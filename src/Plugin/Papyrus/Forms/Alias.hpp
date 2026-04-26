#pragma once

namespace Plugin::Papyrus::Forms::Alias
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Alias");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
