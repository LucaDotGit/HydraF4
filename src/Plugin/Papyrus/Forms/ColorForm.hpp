#pragma once

namespace Plugin::Papyrus::Forms::ColorForm
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:ColorForm");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
