#pragma once

namespace Plugin::Papyrus::Forms::AimModel
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:AimModel");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
