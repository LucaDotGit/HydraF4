#include "Plugin/Papyrus/Forms/Container.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Container
{
	using ContainerFlags = RE::CONT_DATA::Flags;
	using ContainerRecordFlags = RE::TESObjectCONT::RecordFlags;

	static RE::BGSSoundDescriptorForm* GetOpenSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_container->openSound).load(std::memory_order_acquire);
	}

	static void SetOpenSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_container->openSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetCloseSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_container->closeSound).load(std::memory_order_acquire);
	}

	static void SetCloseSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_container->closeSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetTakeAllSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_container->takeAllSound).load(std::memory_order_acquire);
	}

	static void SetTakeAllSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_container->takeAllSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSListForm* GetFilterList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_container->containsOnlyList).load(std::memory_order_acquire);
	}

	static void SetFilterList(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		RE::BGSListForm* a_value)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_container->containsOnlyList).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		ContainerFlags a_flag)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return false;
		}

		return a_container->data.contFlags.any_atomic(a_flag);
	}

	static ContainerFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return ContainerFlags::kNone;
		}

		return a_container->data.contFlags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		ContainerFlags a_flags)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		a_container->data.contFlags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectCONT* a_container,
		ContainerFlags a_flag,
		bool a_set)
	{
		if (!a_container) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONTAINER_NULL, a_stackId);
			return;
		}

		a_container->data.contFlags.set_atomic(a_set, a_flag);
	}

	static ContainerFlags GetFlag_AllowSoundOnAnim(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerFlags::kAllowsSoundsWhenAnimation; }
	static ContainerFlags GetFlag_Respawn(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerFlags::kRespawn; }
	static ContainerFlags GetFlag_ShowOwner(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerFlags::kShowOwner; }

	static ContainerRecordFlags GetRecordFlag_HasDistantLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kHasDistantLOD; }
	static ContainerRecordFlags GetRecordFlag_HasRandomAnimStart(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kRandomAnimStart; }
	static ContainerRecordFlags GetRecordFlag_IsObstacle(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kObstacle; }
	static ContainerRecordFlags GetRecordFlag_HasNavMeshFilter(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kNavMeshGeneration_Filter; }
	static ContainerRecordFlags GetRecordFlag_HasNavMeshBoundingBox(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kNavMeshGeneration_BoundingBox; }
	static ContainerRecordFlags GetRecordFlag_HasNavMeshGround(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ContainerRecordFlags::kNavMeshGeneration_Ground; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOpenSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOpenSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCloseSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCloseSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTakeAllSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTakeAllSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFilterList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFilterList);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowSoundOnAnim);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_Respawn);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_ShowOwner);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasDistantLod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasRandomAnimStart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsObstacle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshFilter);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshBoundingBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshGround);
	}
}