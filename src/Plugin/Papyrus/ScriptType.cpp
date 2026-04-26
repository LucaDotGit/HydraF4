#include "Plugin/Papyrus/ScriptType.hpp"

namespace Plugin::Papyrus::ScriptType
{
	static bool Exists(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName)
	{
		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		return a_vm.GetScriptObjectType(a_scriptName, objectTypeInfo) && objectTypeInfo;
	}

	static bool IsLoaded(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName)
	{
		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		return a_vm.GetScriptObjectTypeNoLoad(a_scriptName, objectTypeInfo) && objectTypeInfo;
	}

	static bool IsInstanceOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_childScriptName,
		RE::BSFixedString a_parentScriptName)
	{
		auto childTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_vm.GetScriptObjectType(a_childScriptName, childTypeInfo) || !childTypeInfo) {
			return false;
		}

		auto parentTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_vm.GetScriptObjectType(a_parentScriptName, parentTypeInfo) || !parentTypeInfo) {
			return false;
		}

		return RE::BSScript::IVirtualMachine::IsInstanceOf(childTypeInfo, parentTypeInfo);
	}

	static RE::BSFixedString GetParentScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName)
	{
		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_vm.GetScriptObjectType(a_scriptName, objectTypeInfo) || !objectTypeInfo) {
			return {};
		}

		const auto parentTypeInfo = objectTypeInfo->parentTypeInfo;
		return parentTypeInfo ? parentTypeInfo->GetName() : RE::BSFixedString();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exists);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLoaded);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsInstanceOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParentScriptName);
	}
}
