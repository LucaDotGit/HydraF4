#pragma once

namespace Plugin::Papyrus::Forms::Book
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Book");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
