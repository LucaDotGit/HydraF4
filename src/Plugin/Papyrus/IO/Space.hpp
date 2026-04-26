#pragma once

namespace Plugin::Papyrus::IO::Space
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:IO:Space");

	using SpaceInfo = RE::BSScript::StructWrapper<SCRIPT_NAME, "SpaceInfo">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
