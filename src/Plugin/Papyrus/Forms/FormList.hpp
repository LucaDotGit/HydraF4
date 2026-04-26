#pragma once

namespace Plugin::Papyrus::Forms::FormList
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:FormList");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
