#pragma once

namespace Plugin::Papyrus::Forms::Container
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Container");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
