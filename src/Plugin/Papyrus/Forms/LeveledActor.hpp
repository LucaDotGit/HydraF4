#pragma once

namespace Plugin::Papyrus::Forms::LeveledActor
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:LeveledActor");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
