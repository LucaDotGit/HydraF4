#pragma once

namespace Plugin::Papyrus::Forms::LeveledList
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:LeveledList");

	using LeveledEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "LeveledEntry">;
	using KeywordEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "KeywordEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
