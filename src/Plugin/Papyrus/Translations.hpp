#pragma once

namespace Plugin::Papyrus::Translations
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Translations");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
