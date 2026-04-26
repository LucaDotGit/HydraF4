#pragma once

namespace Plugin::Papyrus::Forms::Light
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Light");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
