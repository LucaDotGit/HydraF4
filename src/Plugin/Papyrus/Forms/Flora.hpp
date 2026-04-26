#pragma once

namespace Plugin::Papyrus::Forms::Flora
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Flora");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
