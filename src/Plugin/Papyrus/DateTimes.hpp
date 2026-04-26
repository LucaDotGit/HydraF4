#pragma once

namespace Plugin::Papyrus::DateTimes
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:DateTimes");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
