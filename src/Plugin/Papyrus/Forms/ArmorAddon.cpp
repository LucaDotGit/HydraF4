#include "Plugin/Papyrus/Forms/ArmorAddon.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::ArmorAddon
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;
}

namespace Plugin::Papyrus::Forms::ArmorAddon::Impl
{
	[[nodiscard]] __forceinline static std::optional<Vector3Struct> GetBoneScaleModifierByName(
		const std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>& a_boneScaleMap,
		const RE::BSFixedString& a_boneName)
	{
		if (!a_boneScaleMap) {
			return std::nullopt;
		}

		auto boneIt = a_boneScaleMap->find(a_boneName);
		if (boneIt == a_boneScaleMap->end()) {
			return std::nullopt;
		}

		return Vector3Struct::FromNiPoint3(boneIt->second);
	}

	[[nodiscard]] __forceinline static bool SetBoneScaleModifierByName(
		std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>& a_boneScaleMap,
		const RE::BSFixedString& a_boneName,
		const Vector3Struct& a_scale)
	{
		if (!a_boneScaleMap) {
			a_boneScaleMap = std::make_unique<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>();
		}

		const auto&& [_, hasInserted] = a_boneScaleMap->emplace(a_boneName, Vector3Struct::ToNiPoint3(a_scale));
		return hasInserted;
	}
}

namespace Plugin::Papyrus::Forms::ArmorAddon
{
	static std::int8_t GetMalePriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_armorAddon->data.malePriority).load(std::memory_order_acquire);
	}

	static void SetMalePriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		std::int8_t a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_armorAddon->data.malePriority).store(a_value, std::memory_order_release);
	}

	static std::int8_t GetFemalePriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_armorAddon->data.femalePriority).load(std::memory_order_acquire);
	}

	static void SetFemalePriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		std::int8_t a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_armorAddon->data.femalePriority).store(a_value, std::memory_order_release);
	}

	static std::int8_t GetDetectionSoundValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return 0;
		}

		return a_armorAddon->data.detectionSoundValue;
	}

	static void SetDetectionSoundValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		std::int8_t a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->data.detectionSoundValue = a_value;
	}

	static REX::Float32 GetWeaponAdjustment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_armorAddon->data.weaponAdjust;
	}

	static void SetWeaponAdjustment(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		REX::Float32 a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->data.weaponAdjust = a_value;
	}

	static RE::BGSListForm* GetMaleTextureSwapList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return nullptr;
		}

		return a_armorAddon->maleSkinTextureSwapList;
	}

	static void SetMaleTextureSwapList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BGSListForm* a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->maleSkinTextureSwapList = a_value;
	}

	static RE::BGSListForm* GetFemaleTextureSwapList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return nullptr;
		}

		return a_armorAddon->femaleSkinTextureSwapList;
	}

	static void SetFemaleTextureSwapList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BGSListForm* a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->femaleSkinTextureSwapList = a_value;
	}

	static std::optional<RE::BSTArray<RE::TESRace*>> GetAdditionalRaces(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return std::nullopt;
		}

		return a_armorAddon->additionalRaces;
	}

	static void SetAdditionalRaces(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BSTArray<RE::TESRace*> a_values)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->additionalRaces = std::move(a_values);
	}

	static RE::BGSFootstepSet* GetFootstepSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return nullptr;
		}

		return a_armorAddon->footstepSet;
	}

	static void SetFootstepSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BGSFootstepSet* a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->footstepSet = a_value;
	}

	static RE::BGSArtObject* GetArtObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return nullptr;
		}

		return a_armorAddon->artObject;
	}

	static void SetArtObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BGSArtObject* a_value)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return;
		}

		a_armorAddon->artObject = a_value;
	}

	static std::optional<Vector3Struct> GetMaleBoneScaleModifierByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BSFixedString a_name)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& boneScaleMap = reinterpret_cast<const std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>&>(a_armorAddon->maleBoneScaleModifiers);
		return Impl::GetBoneScaleModifierByName(boneScaleMap, a_name);
	}

	static bool SetMaleBoneScaleModifierByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BSFixedString a_name,
		std::optional<Vector3Struct> a_scale)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return false;
		}

		if (!a_scale.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto& boneScaleMap = reinterpret_cast<std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>&>(a_armorAddon->maleBoneScaleModifiers);
		return Impl::SetBoneScaleModifierByName(boneScaleMap, a_name, a_scale.value());
	}

	static std::optional<Vector3Struct> GetFemaleBoneScaleModifierByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BSFixedString a_name)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return std::nullopt;
		}

		const auto& boneScaleMap = reinterpret_cast<const std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>&>(a_armorAddon->femaleBoneScaleModifiers);
		return Impl::GetBoneScaleModifierByName(boneScaleMap, a_name);
	}

	static bool SetFemaleBoneScaleModifierByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectARMA* a_armorAddon,
		RE::BSFixedString a_name,
		std::optional<Vector3Struct> a_scale)
	{
		if (!a_armorAddon) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARMOR_ADDON_NULL, a_stackId);
			return false;
		}

		if (!a_scale.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto& boneScaleMap = reinterpret_cast<std::unique_ptr<RE::BSTHashMap<RE::BSFixedString, RE::NiPoint3>>&>(a_armorAddon->femaleBoneScaleModifiers);
		return Impl::SetBoneScaleModifierByName(boneScaleMap, a_name, a_scale.value());
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMalePriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMalePriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFemalePriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFemalePriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDetectionSoundValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDetectionSoundValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWeaponAdjustment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWeaponAdjustment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMaleTextureSwapList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMaleTextureSwapList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFemaleTextureSwapList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFemaleTextureSwapList);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAdditionalRaces);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetAdditionalRaces);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFootstepSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFootstepSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetArtObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetArtObject);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetMaleBoneScaleModifierByName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetMaleBoneScaleModifierByName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFemaleBoneScaleModifierByName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFemaleBoneScaleModifierByName);
	}
}
