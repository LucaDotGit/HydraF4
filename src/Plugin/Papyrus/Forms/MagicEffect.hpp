#pragma once

namespace Plugin::Papyrus::Forms::MagicEffect
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:MagicEffect");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
