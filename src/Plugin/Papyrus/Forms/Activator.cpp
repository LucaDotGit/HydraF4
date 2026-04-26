#include "Plugin/Papyrus/Forms/Activator.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Activator
{
	using ActivatorFlags = RE::TESObjectACTI::Flags;
	using ActivatorRecordFlags = RE::TESObjectACTI::RecordFlags;

	static RE::BGSSoundDescriptorForm* GetLoopSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_activator->soundLoop).load(std::memory_order_acquire);
	}

	static void SetLoopSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_activator->soundLoop).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetActivateSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_activator->soundActivate).load(std::memory_order_acquire);
	}

	static void SetActivateSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_activator->soundActivate).store(a_value, std::memory_order_release);
	}

	static RE::TESWaterForm* GetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_activator->waterForm).load(std::memory_order_acquire);
	}

	static void SetWaterType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		RE::TESWaterForm* a_value)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_activator->waterForm).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		ActivatorFlags a_flag)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return false;
		}

		return a_activator->flags.any_atomic(a_flag);
	}

	static ActivatorFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return ActivatorFlags::kNone;
		}

		return a_activator->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		ActivatorFlags a_flags)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return;
		}

		a_activator->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectACTI* a_activator,
		ActivatorFlags a_flag,
		bool a_set)
	{
		if (!a_activator) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTIVATOR_NULL, a_stackId);
			return;
		}

		a_activator->flags.set_atomic(a_set, a_flag);
	}

	static ActivatorFlags GetFlag_HasNoDisplacement(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorFlags::kNoDisplacement; }
	static ActivatorFlags GetFlag_IsIgnoredBySandbox(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorFlags::kIgnoredBySandbox; }
	static ActivatorFlags GetFlag_IsProceduralWater(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorFlags::kIsProceduralWater; }
	static ActivatorFlags GetFlag_IsLodWater(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorFlags::kIsLODWater; }
	static ActivatorFlags GetFlag_IsRadio(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorFlags::kIsRadio; }

	static ActivatorRecordFlags GetRecordFlag_NeverFade(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kNeverFades; }
	static ActivatorRecordFlags GetRecordFlag_IsNonOccluder(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kNonOccluder; }
	static ActivatorRecordFlags GetRecordFlag_HasTreeLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kHasTreeLOD; }
	static ActivatorRecordFlags GetRecordFlag_IsHeadingMarker(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kHeadingMarker; }
	static ActivatorRecordFlags GetRecordFlag_MustUpdateAnims(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kMustUpdateAnims; }
	static ActivatorRecordFlags GetRecordFlag_IsHiddenFromLocalMap(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kHiddenFromLocalMap; }
	static ActivatorRecordFlags GetRecordFlag_IsHeadtrackMarker(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kHeadtrackMarker; }
	static ActivatorRecordFlags GetRecordFlag_IsUsedAsPlatform(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kUsedAsPlatform; }
	static ActivatorRecordFlags GetRecordFlag_IsForPackInUseOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kPackInUseOnly; }
	static ActivatorRecordFlags GetRecordFlag_HasDistantLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kHasDistantLOD; }
	static ActivatorRecordFlags GetRecordFlag_HasRandomAnimStart(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kRandomAnimStart; }
	static ActivatorRecordFlags GetRecordFlag_IsDangerous(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kDangerous; }
	static ActivatorRecordFlags GetRecordFlag_IgnoreObjectInteraction(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kIgnoresObjectInteraction; }
	static ActivatorRecordFlags GetRecordFlag_IsMarker(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kIsMarker; }
	static ActivatorRecordFlags GetRecordFlag_IsObstacle(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kObstacle; }
	static ActivatorRecordFlags GetRecordFlag_HasNavMeshFilter(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kNavMeshGeneration_Filter; }
	static ActivatorRecordFlags GetRecordFlag_HasNavMeshBoundingBox(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kNavMeshGeneration_BoundingBox; }
	static ActivatorRecordFlags GetRecordFlag_IsUsableByChildren(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kChildCanUse; }
	static ActivatorRecordFlags GetRecordFlag_HasNavMeshGround(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ActivatorRecordFlags::kNavMeshGeneration_Ground; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLoopSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLoopSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActivateSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetActivateSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWaterType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWaterType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoDisplacement);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsIgnoredBySandbox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsProceduralWater);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsLodWater);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsRadio);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_NeverFade);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsNonOccluder);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasTreeLod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsHeadingMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_MustUpdateAnims);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsHiddenFromLocalMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsHeadtrackMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsUsedAsPlatform);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsForPackInUseOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasDistantLod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasRandomAnimStart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsDangerous);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IgnoreObjectInteraction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsMarker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsObstacle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshFilter);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshBoundingBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsUsableByChildren);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasNavMeshGround);
	}
}
