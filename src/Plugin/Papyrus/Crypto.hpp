#pragma once

namespace Plugin::Papyrus::Crypto
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Crypto");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
