#pragma once

namespace Plugin::PapyrusManager
{
	void Setup(REX::NotNull<const F4SE::PapyrusInterface*> a_script);
	bool Callback(RE::BSScript::IVirtualMachine* a_vm);
	void RegisterTypes(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
