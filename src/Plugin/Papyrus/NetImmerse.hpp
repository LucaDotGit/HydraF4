#pragma once

namespace Plugin::Papyrus::NetImmerse
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:NetImmerse");

	using NiMatrix4x3 = RE::BSScript::StructWrapper<SCRIPT_NAME, "NiMatrix4x3">;
	using NiTransform = RE::BSScript::StructWrapper<SCRIPT_NAME, "NiTransform">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
