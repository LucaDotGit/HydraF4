#pragma once

namespace Plugin::Papyrus::IO::File
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:File");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
