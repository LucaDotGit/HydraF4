#pragma once

namespace Plugin::Papyrus::Forms::HeadPart
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:HeadPart");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
