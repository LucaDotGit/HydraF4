#pragma once

namespace Plugin::Papyrus::Forms::MagicItem
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:MagicItem");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
