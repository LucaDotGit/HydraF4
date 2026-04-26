#pragma once

namespace Plugin::Papyrus::StructObject
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:StructObject");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
