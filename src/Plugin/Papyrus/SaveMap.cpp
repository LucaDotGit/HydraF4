#include "Plugin/Papyrus/SaveMap.hpp"

#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp"
#include "Plugin/Internal/SaveManager.hpp"
#include "Plugin/Internal/Structs/VarPairs.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::SaveMap
{
	using VarPairStruct = ::Plugin::Internal::Structs::VarPairs::VarPairData;

	static bool IsNamespaceValid(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->IsNamespaceValid(a_namespace);
	}

	static std::vector<RE::BSFixedString> GetNamespaces(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetSortedNamespaces();
	}

	static std::vector<RE::BSScript::UniqueVariant> GetKeys(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetSortedKeys(a_namespace);
	}

	static std::vector<RE::BSScript::UniqueVariant> GetValues(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetSortedValues(a_namespace);
	}

	static std::vector<std::optional<VarPairStruct>> GetPairs(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetPairs(a_namespace);
	}

	static std::size_t GetNamespaceSize(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetNamespaceSize();
	}

	static std::size_t GetKeySize(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetKeySize(a_namespace);
	}

	static bool SetPairs(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		std::vector<std::optional<VarPairStruct>> a_pairs)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->SetPairs(a_namespace, a_pairs);
	}

	static bool ContainsNamespace(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->ContainsNamespace(a_namespace);
	}

	static bool ContainsKey(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->ContainsKey(a_namespace, a_key);
	}

	static RE::BSScript::UniqueVariant GetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetValue(a_namespace, a_key);
	}

	static RE::BSScript::UniqueVariant GetValueOrDefault(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_default)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetValueOrDefault(a_namespace, a_key, a_default);
	}

	static bool SetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->SetValue(a_namespace, a_key, a_value);
	}

	static bool Update(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_newValue,
		RE::BSScript::SharedVariant a_compareValue)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->Update(a_namespace, a_key, a_newValue, a_compareValue);
	}

	static bool Add(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->Add(a_namespace, a_key, a_value);
	}

	static RE::BSScript::UniqueVariant AddOrUpdate(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_addValue,
		RE::BSScript::SharedVariant a_updateValue)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->AddOrUpdate(a_namespace, a_key, a_addValue, a_updateValue);
	}

	static RE::BSScript::UniqueVariant GetOrAdd(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->GetOrAdd(a_namespace, a_key, a_value);
	}

	static bool RemoveKey(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->RemoveKey(a_namespace, a_key);
	}

	static bool RemoveNamespace(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace)
	{
		return ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap()->RemoveNamespace(a_namespace);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNamespaceValid);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNamespaces);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeys);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValues);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPairs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPairs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNamespaceSize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeySize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsNamespace);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsKey);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValueOrDefault);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Update);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Add);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AddOrUpdate);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOrAdd);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveKey);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemoveNamespace);
	}
}
