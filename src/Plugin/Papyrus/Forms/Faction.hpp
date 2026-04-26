#pragma once

namespace Plugin::Papyrus::Forms::Faction
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Faction");

	using RelationEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "RelationEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
