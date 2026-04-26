#pragma once

namespace Plugin::Papyrus::Forms::Form
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Form");

	using ContainerEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "ContainerEntry">;
	using PropertyEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "PropertyEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
