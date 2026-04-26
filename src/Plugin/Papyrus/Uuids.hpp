#pragma once

namespace Plugin::Papyrus::Uuids
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Uuids");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
