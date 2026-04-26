#pragma once

namespace Plugin::Papyrus::Forms::Actor
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Actor");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
