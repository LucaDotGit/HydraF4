#pragma once

namespace Plugin::Papyrus::Forms::MovementType
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:MovementType");

	using SpeedEntry = RE::BSScript::StructWrapper<SCRIPT_NAME, "SpeedEntry">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
