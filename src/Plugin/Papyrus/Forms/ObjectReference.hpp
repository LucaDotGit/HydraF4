#pragma once

namespace Plugin::Papyrus::Forms::ObjectReference
{
	inline constexpr auto SCRIPT_NAME = REX::StaticString("Hydra:Forms:ObjectReference");

	using MapMarkerData = RE::BSScript::StructWrapper<SCRIPT_NAME, "MapMarkerData">;
	using DoorDestinationData = RE::BSScript::StructWrapper<SCRIPT_NAME, "DoorDestinationData">;
	using AliasInstanceData = RE::BSScript::StructWrapper<SCRIPT_NAME, "AliasInstanceData">;

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm);
}
