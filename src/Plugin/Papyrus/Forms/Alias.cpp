#include "Plugin/Papyrus/Forms/Alias.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Alias
{
	using AliasFlags = RE::BGSBaseAlias::Flags;

	static std::uint32_t GetId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return std::numeric_limits<std::uint32_t>::max();
		}

		return std::atomic_ref(a_alias->aliasID).load(std::memory_order_acquire);
	}

	static RE::BSFixedString GetName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return {};
		}

		return a_alias->aliasName;
	}

	static RE::TESQuest* GetOwningQuest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_alias->owningQuest).load(std::memory_order_acquire);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias,
		AliasFlags a_flag)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return false;
		}

		return a_alias->flags.any_atomic(a_flag);
	}

	static AliasFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return AliasFlags::kNone;
		}

		return a_alias->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias,
		AliasFlags a_flag)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return;
		}

		a_alias->flags.store_atomic(a_flag);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSBaseAlias* a_alias,
		AliasFlags a_flag,
		bool a_set)
	{
		if (!a_alias) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ALIAS_NULL, a_stackId);
			return;
		}

		a_alias->flags.set_atomic(a_set, a_flag);
	}

	static AliasFlags GetFlag_Reserve(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kReserves; }
	static AliasFlags GetFlag_IsOptional(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kOptional; }
	static AliasFlags GetFlag_IsQuestObject(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kQuestObject; }
	static AliasFlags GetFlag_AllowReuse(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowReuse; }
	static AliasFlags GetFlag_AllowDead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowDead; }
	static AliasFlags GetFlag_AllowLoadedOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kLoadedOnly; }
	static AliasFlags GetFlag_IsEssential(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kEssential; }
	static AliasFlags GetFlag_AllowDisabled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowDisabled; }
	static AliasFlags GetFlag_StoreName(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kStoreName; }
	static AliasFlags GetFlag_AllowReserved(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowReserved; }
	static AliasFlags GetFlag_IsProtected(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kProtected; }
	static AliasFlags GetFlag_IsForcedAlias(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kForcedFromAlias; }
	static AliasFlags GetFlag_AllowDestroyed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowDestroyed; }
	static AliasFlags GetFlag_FindRefClosest(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kFindClosestRef; }
	static AliasFlags GetFlag_UseName(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kUsesNames; }
	static AliasFlags GetFlag_IsInitiallyDisabled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kInitiallyDisabled; }
	static AliasFlags GetFlag_AllowCleared(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kAllowCleared; }
	static AliasFlags GetFlag_ClearNameOnRemoval(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kClearNameOnRemove; }
	static AliasFlags GetFlag_AllowActorsOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kActorsOnly; }
	static AliasFlags GetFlag_IsTransient(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kTransient; }
	static AliasFlags GetFlag_IsLinkExternal(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kExternalLink; }
	static AliasFlags GetFlag_DisallowPickpocketing(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kNoPickPocket; }
	static AliasFlags GetFlag_IsDataAlias(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kDataAlias; }
	static AliasFlags GetFlag_IsCompanion(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kCompanion; }
	static AliasFlags GetFlag_IsSceneOptional(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AliasFlags::kSceneOptional; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOwningQuest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_Reserve);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsOptional);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsQuestObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowReuse);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowDead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowLoadedOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsEssential);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowDisabled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_StoreName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowReserved);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsProtected);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsForcedAlias);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowDestroyed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_FindRefClosest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UseName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsInitiallyDisabled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowCleared);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_ClearNameOnRemoval);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllowActorsOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsTransient);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsLinkExternal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisallowPickpocketing);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDataAlias);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsCompanion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSceneOptional);
	}
}
