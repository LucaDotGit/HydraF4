#pragma once

namespace Plugin::Papyrus::IO::Json
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Json");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
