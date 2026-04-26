#pragma once

namespace Plugin::Papyrus::StructType
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:StructType");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
