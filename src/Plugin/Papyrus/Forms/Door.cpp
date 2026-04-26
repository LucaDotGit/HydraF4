#include "Plugin/Papyrus/Forms/Door.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Forms::Door
{
	using DoorFlags = RE::TESObjectDOOR::Flags;
	using DoorRecordFlags = RE::TESObjectDOOR::RecordFlags;

	static RE::BGSSoundDescriptorForm* GetOpenSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_door->openSound).load(std::memory_order_acquire);
	}

	static void SetOpenSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_door->openSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetCloseSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_door->closeSound).load(std::memory_order_acquire);
	}

	static void SetCloseSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_door->closeSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetLoopSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_door->loopSound).load(std::memory_order_acquire);
	}

	static void SetLoopSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_door->loopSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSLocalizedString GetAltOpenText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return {};
		}

		return a_door->altOpenText;
	}

	static void SetAltOpenText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		a_door->altOpenText = a_value.GetValue();
	}

	static RE::BGSLocalizedString GetAltCloseText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return {};
		}

		return a_door->altCloseText;
	}

	static void SetAltCloseText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		a_door->altCloseText = RE::BGSLocalizedString(a_value.GetValue());
	}

	static bool ContainsRandomTeleporter(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		RE::TESForm* a_teleporter)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return false;
		}

		return std::ranges::any_of(a_door->randomTeleports, [&a_teleporter](RE::TESForm* a_form) {
			return a_form == a_teleporter;
		});
	}

	static std::optional<RE::BSTArray<RE::TESForm*>> GetRandomTeleporters(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return std::nullopt;
		}

		return a_door->randomTeleports;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		DoorFlags a_flag)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return false;
		}

		return a_door->flags.any_atomic(a_flag);
	}

	static DoorFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return DoorFlags::kNone;
		}

		return a_door->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		DoorFlags a_flags)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		a_door->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectDOOR* a_door,
		DoorFlags a_flag,
		bool a_set)
	{
		if (!a_door) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::DOOR_NULL, a_stackId);
			return;
		}

		a_door->flags.set_atomic(a_set, a_flag);
	}

	static DoorFlags GetFlag_IsAutomatic(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kAutomatic; }
	static DoorFlags GetFlag_IsHidden(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kHidden; }
	static DoorFlags GetFlag_IsUsedMinimally(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kMinimalUse; }
	static DoorFlags GetFlag_IsSlidable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kSliding; }
	static DoorFlags GetFlag_DisallowOpeningInCombatSearch(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kDoNotOpenInCombatSearch; }
	static DoorFlags GetFlag_HasNoDestinationText(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorFlags::kNoToText; }

	static DoorRecordFlags GetRecordFlag_HasDistantLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorRecordFlags::kHasDistantLOD; }
	static DoorRecordFlags GetRecordFlag_HasRandomAnimStart(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorRecordFlags::kRandomAnimStart; }
	static DoorRecordFlags GetRecordFlag_IsMarker(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DoorRecordFlags::kIsMarker; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOpenSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOpenSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCloseSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCloseSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLoopSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLoopSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAltOpenText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAltOpenText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAltCloseText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAltCloseText);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsRandomTeleporter);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRandomTeleporters);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsAutomatic);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHidden);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsUsedMinimally);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSlidable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisallowOpeningInCombatSearch);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoDestinationText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasDistantLod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasRandomAnimStart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsMarker);
	}
}