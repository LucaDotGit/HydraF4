#pragma once

namespace Plugin::Papyrus::Forms::ArmorAddon
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:ArmorAddon");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
