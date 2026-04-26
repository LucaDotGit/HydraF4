#pragma once

namespace Plugin::Papyrus::Forms::Location
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Location");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
