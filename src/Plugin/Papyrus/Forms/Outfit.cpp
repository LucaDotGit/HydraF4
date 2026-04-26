#include "Plugin/Papyrus/Forms/Outfit.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Outfit
{
	static std::optional<RE::BSTArray<RE::TESForm*>> GetParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSOutfit* a_outfit)
	{
		if (!a_outfit) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUTFIT_NULL, a_stackId);
			return std::nullopt;
		}

		return a_outfit->outfitItems;
	}

	static void SetParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSOutfit* a_outfit,
		RE::BSTArray<RE::TESForm*> a_values)
	{
		if (!a_outfit) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUTFIT_NULL, a_stackId);
			return;
		}

		a_outfit->outfitItems = std::move(a_values);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetParts);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetParts);
	}
}
