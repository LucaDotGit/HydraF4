#pragma once

namespace Plugin::Papyrus::Forms::InstanceNamingRules
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:InstanceNamingRules");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
