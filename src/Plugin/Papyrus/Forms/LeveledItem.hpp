#pragma once

namespace Plugin::Papyrus::Forms::LeveledItem
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:LeveledItem");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
