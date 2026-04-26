#include "Plugin/Papyrus/MemSet.hpp"

#include "Plugin/Internal/Script/ScriptVariantSet.hpp"
#include "Plugin/Internal/ScriptManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::MemSet
{
	static std::vector<RE::BSFixedString> GetNamespaces(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->GetSortedNamespaces();
	}

	static std::vector<RE::BSScript::UniqueVariant> GetKeys(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->GetSortedKeys(a_namespace);
	}

	static std::size_t GetNamespaceSize(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->GetNamespaceSize();
	}

	static bool ContainsNamespace(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->ContainsNamespace(a_namespace);
	}

	static bool ContainsKey(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->ContainsKey(a_namespace, a_key);
	}

	static bool Add(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->Add(a_namespace, a_key);
	}

	static bool AddRange(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		std::vector<RE::BSScript::SharedVariant> a_key)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->AddRange(a_namespace, a_key);
	}

	static bool RemoveKey(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->RemoveKey(a_namespace, a_key);
	}

	static bool RemoveNamespace(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetScriptManager()->GetMemorySet()->RemoveNamespace(a_namespace);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNamespaces);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeys);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNamespaceSize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsNamespace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsKey);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Add);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AddRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveKey);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveNamespace);
	}
}
