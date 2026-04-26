#pragma once

namespace Plugin::Papyrus::IO::Directory
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Directory");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
