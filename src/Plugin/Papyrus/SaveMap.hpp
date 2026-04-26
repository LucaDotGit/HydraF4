#pragma once

namespace Plugin::Papyrus::SaveMap
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:SaveMap");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
