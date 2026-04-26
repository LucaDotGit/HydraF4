#pragma once

namespace Plugin::Papyrus::Camera
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Camera");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
