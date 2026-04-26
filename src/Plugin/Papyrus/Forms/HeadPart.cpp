#include "Plugin/Papyrus/Forms/HeadPart.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::HeadPart
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;
	using HeadPartFlags = RE::BGSHeadPart::Flags;

	static HeadPartType GetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return static_cast<HeadPartType>(-1);
		}

		return a_headPart->type.get_atomic();
	}

	static void SetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		HeadPartType a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		a_headPart->type.store_atomic(a_value);
	}

	static HeadPartType GetType_Misc(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kMisc; }
	static HeadPartType GetType_Face(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kFace; }
	static HeadPartType GetType_Eyes(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kEyes; }
	static HeadPartType GetType_Hair(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kHair; }
	static HeadPartType GetType_FacialHair(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kFacialHair; }
	static HeadPartType GetType_Scar(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kScar; }
	static HeadPartType GetType_Eyebrows(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kEyebrows; }
	static HeadPartType GetType_Meatcaps(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kMeatcaps; }
	static HeadPartType GetType_Teeth(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kTeeth; }
	static HeadPartType GetType_HeadRear(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartType::kHeadRear; }

	static std::optional<RE::BSTArray<RE::BGSHeadPart*>> GetExtraParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return std::nullopt;
		}

		return a_headPart->extraParts;
	}

	static void SetExtraParts(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		RE::BSTArray<RE::BGSHeadPart*> a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		a_headPart->extraParts = std::move(a_value);
	}

	static RE::BGSTextureSet* GetTextureSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_headPart->textureSet).load(std::memory_order_acquire);
	}

	static void SetTextureSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		RE::BGSTextureSet* a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_headPart->textureSet).store(a_value, std::memory_order_release);
	}

	static RE::BGSColorForm* GetColorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_headPart->colorForm).load(std::memory_order_acquire);
	}

	static void SetColorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		RE::BGSColorForm* a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_headPart->colorForm).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetValidRaceList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_headPart->validRaces).load(std::memory_order_acquire);
	}

	static void SetValidRaceList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		RE::BGSListForm* a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_headPart->validRaces).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		HeadPartFlags a_flag)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return false;
		}

		return a_headPart->flags.any_atomic(a_flag);
	}

	static HeadPartFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return HeadPartFlags::kNone;
		}

		return a_headPart->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		HeadPartFlags a_value)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		a_headPart->flags.set_atomic(a_value);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSHeadPart* a_headPart,
		HeadPartFlags a_flag,
		bool a_set)
	{
		if (!a_headPart) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HEAD_PART_NULL, a_stackId);
			return;
		}

		a_headPart->flags.set_atomic(a_set, a_flag);
	}

	static HeadPartFlags GetFlag_IsPlayable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kPlayable; }
	static HeadPartFlags GetFlag_CannotBeMale(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kCantBeMale; }
	static HeadPartFlags GetFlag_CannotBeFemale(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kCantBeFemale; }
	static HeadPartFlags GetFlag_IsExtraPart(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kExtraPart; }
	static HeadPartFlags GetFlag_UseSolidTint(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kUseSolidTint; }
	static HeadPartFlags GetFlag_UseTextureTint(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return HeadPartFlags::kUseBodyTexture; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Misc);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Face);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Eyes);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Hair);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_FacialHair);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Scar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Eyebrows);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Meatcaps);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Teeth);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_HeadRear);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetExtraParts);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetExtraParts);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTextureSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTextureSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetColorForm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetColorForm);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValidRaceList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetValidRaceList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPlayable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CannotBeMale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CannotBeFemale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsExtraPart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseSolidTint);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseTextureTint);
	}
}
