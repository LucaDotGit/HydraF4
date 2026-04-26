#pragma once

namespace Plugin::Papyrus::SaveSet
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:SaveSet");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
