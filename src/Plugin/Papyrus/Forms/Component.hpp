#pragma once

namespace Plugin::Papyrus::Forms::Component
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Component");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
