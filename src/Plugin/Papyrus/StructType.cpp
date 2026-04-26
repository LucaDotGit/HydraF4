#include "Plugin/Papyrus/StructType.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Pairs.hpp"

namespace Plugin::Papyrus::StructType
{
	using PairStruct = ::Plugin::Internal::Structs::Pairs::PairData;

	static bool Exists(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		return a_vm.GetScriptStructType(a_structName, structTypeInfo) && structTypeInfo;
	}

	static bool IsLoaded(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		return a_vm.GetScriptStructTypeNoLoad(a_structName, structTypeInfo);
	}

	static bool ContainsVariable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName,
		RE::BSFixedString a_variableName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_vm.GetScriptStructType(a_structName, structTypeInfo) || !structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NAME_NOT_FOUND, a_stackId, a_structName);
			return false;
		}

		const auto& varPairMap = structTypeInfo->varNameIndexMap;
		return varPairMap.contains(a_variableName);
	}

	static RE::BSFixedString GetParentScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_vm.GetScriptStructType(a_structName, structTypeInfo) || !structTypeInfo) {
			return {};
		}

		const auto parentTypeInfo = structTypeInfo->GetParentTypeInfo();
		return parentTypeInfo ? parentTypeInfo->GetName() : RE::BSFixedString();
	}

	static std::optional<std::vector<std::optional<PairStruct>>> GetInitialValues(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_vm.GetScriptStructType(a_structName, structTypeInfo)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NAME_NOT_FOUND, a_stackId, a_structName);
			return std::nullopt;
		}

		const auto& varPairMap = structTypeInfo->varNameIndexMap;

		auto variables = std::vector<std::optional<PairStruct>>();
		variables.reserve(varPairMap.size());

		for (const auto& [varName, varIndex] : varPairMap) {
			if (varIndex >= structTypeInfo->variables.size()) [[unlikely]] {
				REX::Assert(false);
				continue;
			}

			const auto& varElement = structTypeInfo->variables[varIndex];

			auto pair = std::make_pair(varName, varElement.initialValue);
			variables.emplace_back(std::move(pair));
		}

		return variables;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exists);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLoaded);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsVariable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParentScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInitialValues);
	}
}
