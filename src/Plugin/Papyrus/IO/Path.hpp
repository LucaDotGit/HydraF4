#pragma once

namespace Plugin::Papyrus::IO::Path
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Path");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
