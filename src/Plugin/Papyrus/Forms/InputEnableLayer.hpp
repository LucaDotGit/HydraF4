#pragma once

namespace Plugin::Papyrus::Forms::InputEnableLayer
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:InputEnableLayer");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
