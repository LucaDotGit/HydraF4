#pragma once

namespace Plugin::Papyrus::IO::Link
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Link");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
