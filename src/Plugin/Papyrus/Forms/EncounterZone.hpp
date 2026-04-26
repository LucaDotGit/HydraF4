#pragma once

namespace Plugin::Papyrus::Forms::EncounterZone
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:EncounterZone");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
