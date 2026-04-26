#pragma once

namespace Plugin::Papyrus::Forms::ActorBase
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:ActorBase");

	using FactionRank = RE::BSScript::StructWrapper<SCRIPT_NAME, "FactionRank">;
	using PerkRank = RE::BSScript::StructWrapper<SCRIPT_NAME, "PerkRank">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
