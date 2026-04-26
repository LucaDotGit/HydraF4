#pragma once

namespace Plugin::Papyrus::Forms::MiscObject
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:MiscObject");

	using ComponentEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "ComponentEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
