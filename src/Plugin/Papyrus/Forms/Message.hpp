#pragma once

namespace Plugin::Papyrus::Forms::Message
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:Message");

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
