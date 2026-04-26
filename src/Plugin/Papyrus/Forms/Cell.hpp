#pragma once

namespace Plugin::Papyrus::Forms::Cell
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Cell");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
