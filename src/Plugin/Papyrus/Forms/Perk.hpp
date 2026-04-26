#pragma once

namespace Plugin::Papyrus::Forms::Perk
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Perk");

	using Entry = RE::BSScript::StructWrapper<SCRIPT_NAME, "Entry">;
	using QuestEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "QuestEntry">;
	using ValueTwoEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "ValueTwoEntry">;
	using ActorValueEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "ActorValueEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
