#include "Plugin/Papyrus/StructObject.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Pairs.hpp"

namespace Plugin::Papyrus::StructObject
{
	using PairStruct = ::Plugin::Internal::Structs::Pairs::PairData;

	static RE::BSScript::UniqueVariant Create(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_structName,
		std::vector<std::optional<PairStruct>> a_variables)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_vm.GetScriptStructType(a_structName, structTypeInfo) || !structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NAME_NOT_FOUND, a_stackId, a_structName);
			return nullptr;
		}

		auto structure = RE::BSTSmartPointer<RE::BSScript::Struct>();
		if (!a_vm.CreateStruct(structTypeInfo->GetName(), structure) || !structure) {
			return nullptr;
		}

		const auto structLock = RE::BSAutoLock(structure->structLock);

		for (const auto& variable : a_variables) {
			if (!variable.has_value()) {
				continue;
			}

			const auto& [varName, varValue] = variable.value();

			const auto varIndex = structTypeInfo->GetVariableIndex(varName);
			if (!varIndex.has_value()) {
				continue;
			}

			auto& element = structure->variables[varIndex.value()];
			if (!element.TryAssign(varValue)) {
				continue;
			}
		}

		return RE::BSScript::CreateVariant(std::move(structure));
	}

	static RE::BSScript::UniqueVariant Copy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct)
	{
		if (!a_struct) {
			return nullptr;
		}

		if (!a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return nullptr;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(structure->Clone(a_vm));
	}

	static RE::BSScript::UniqueVariant DeepCopy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct)
	{
		if (!a_struct) {
			return nullptr;
		}

		if (!a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return nullptr;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(structure->DeepClone(a_vm));
	}

	static std::int32_t Compare(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		if (a_left == a_right) {
			return true;
		}

		if (!a_left || !a_right) {
			return false;
		}

		if (!a_left->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return false;
		}

		if (!a_right->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return false;
		}

		const auto leftStruct = a_left->GetStruct();
		const auto rightStruct = a_right->GetStruct();

		if (leftStruct == rightStruct) {
			return true;
		}

		if (!leftStruct || !rightStruct) {
			return false;
		}

		const auto comparison = leftStruct->Compare(*rightStruct);
		return REX::GetNumericStrongOrdering(comparison);
	}

	static std::int32_t DeepCompare(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		if (a_left == a_right) {
			return true;
		}

		if (!a_left || !a_right) {
			return false;
		}

		if (!a_left->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return false;
		}

		if (!a_right->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return false;
		}

		const auto leftStruct = a_left->GetStruct();
		const auto rightStruct = a_right->GetStruct();

		if (leftStruct == rightStruct) {
			return true;
		}

		if (!leftStruct || !rightStruct) {
			return false;
		}

		const auto comparison = leftStruct->DeepCompare(*rightStruct);
		return REX::GetNumericPartialOrdering(comparison);
	}

	static bool Equals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		if (a_left == a_right) {
			return true;
		}

		if (!a_left || !a_right) {
			return false;
		}

		if (!a_left->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return false;
		}

		if (!a_right->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return false;
		}

		const auto leftStruct = a_left->GetStruct();
		const auto rightStruct = a_right->GetStruct();

		if (leftStruct == rightStruct) {
			return true;
		}

		if (!leftStruct || !rightStruct) {
			return false;
		}

		return leftStruct->Equals(*rightStruct);
	}

	static bool DeepEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		if (a_left == a_right) {
			return true;
		}

		if (!a_left || !a_right) {
			return false;
		}

		if (!a_left->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return false;
		}

		if (!a_right->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return false;
		}

		const auto leftStruct = a_left->GetStruct();
		const auto rightStruct = a_right->GetStruct();

		if (leftStruct == rightStruct) {
			return true;
		}

		if (!leftStruct || !rightStruct) {
			return false;
		}

		return leftStruct->DeepEquals(*rightStruct);
	}

	static bool IsInstanceOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct,
		RE::BSFixedString a_structName)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto structTypeInfo = structure->GetTypeInfo();
		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		return structTypeInfo->GetName() == a_structName;
	}

	static RE::BSFixedString GetStructName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		const auto structTypeInfo = structure->GetTypeInfo();
		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		return structTypeInfo->GetName();
	}

	static RE::BSFixedString GetParentScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		const auto structTypeInfo = structure->GetTypeInfo();
		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		const auto& containingTypeInfo = structTypeInfo->containingObjTypeInfo;
		if (!containingTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return {};
		}

		return containingTypeInfo->GetName();
	}

	static std::optional<std::vector<std::optional<PairStruct>>> GetVariables(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto structLock = RE::BSAutoLock(structure->structLock);
		const auto structTypeInfo = structure->GetTypeInfo();

		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return std::nullopt;
		}

		auto variables = std::vector<std::optional<PairStruct>>();
		variables.reserve(structTypeInfo->variables.size());

		for (const auto& [varName, varIndex] : structTypeInfo->varNameIndexMap) {
			if (varIndex >= structTypeInfo->variables.size()) [[unlikely]] {
				REX::Assert(false);
				continue;
			}

			const auto& varElement = structure->variables[varIndex];

			auto pair = std::make_pair(varName, varElement);
			variables.emplace_back(std::move(pair));
		}

		return variables;
	}

	static RE::BSScript::UniqueVariant GetVariableValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct,
		RE::BSFixedString a_variableName)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return nullptr;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return nullptr;
		}

		const auto structLock = RE::BSAutoLock(structure->structLock);
		const auto structTypeInfo = structure->GetTypeInfo();

		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return nullptr;
		}

		const auto* variable = structure->GetVariable(a_variableName);
		if (!variable) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::VARIABLE_NOT_FOUND, a_stackId, a_variableName);
			return nullptr;
		}

		return RE::BSScript::CreateVariant(variable);
	}

	static bool SetVariableValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_struct,
		RE::BSFixedString a_variableName,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_struct || !a_struct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto structure = a_struct->GetStruct();
		if (!structure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto structLock = RE::BSAutoLock(structure->structLock);

		const auto structTypeInfo = structure->GetTypeInfo();
		if (!structTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* variable = structure->GetVariable(a_variableName);
		if (!variable) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::VARIABLE_NOT_FOUND, a_stackId, a_variableName);
			return false;
		}

		return variable->TryAssign(RE::BSScript::UnwrapVariant(a_value));
	}

	static RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> GetValuesByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_structs,
		RE::BSFixedString a_variableName)
	{
		if (!a_structs) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return nullptr;
		}

		auto values = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>::Create();
		values.elements().reserve(a_structs.size());

		for (const auto& element : a_structs) {
			if (!element.IsVar()) {
				continue;
			}

			const auto* elementVar = element.GetVar();
			if (!elementVar || !elementVar->IsStruct()) {
				continue;
			}

			const auto elementStruct = elementVar->GetStruct();
			if (!elementStruct) {
				continue;
			}

			const auto structLock = RE::BSAutoLock(elementStruct->structLock);

			const auto* variable = elementStruct->GetVariable(a_variableName);
			if (!variable) {
				continue;
			}

			auto variant = RE::BSScript::CreateVariant(variable);
			values.elements().emplace_back(std::move(variant));
		}

		return values;
	}

	static std::int32_t IndexOfValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_structs,
		RE::BSFixedString a_variableName,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_structs) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return -1;
		}

		const auto size = static_cast<std::int32_t>(a_structs.size());
		if (a_startIndex < 0 || a_startIndex >= size) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_OUT_OF_RANGE, a_stackId, a_startIndex, 0, size);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		a_count = std::min(a_count, size);

		const auto* varBegin = a_structs.begin() + a_startIndex;
		const auto* varEnd = a_structs.begin() + std::min(size, a_startIndex + a_count);

		const auto* varIt = std::find_if(varBegin, varEnd, [&a_variableName, &a_value](const RE::BSScript::Variable& a_element) {
			if (!a_element.IsVar()) {
				return false;
			}

			const auto* elementVar = a_element.GetVar();
			if (!elementVar || !elementVar->IsStruct()) {
				return false;
			}

			const auto elementStruct = elementVar->GetStruct();
			if (!elementStruct) {
				return false;
			}

			const auto structLock = RE::BSAutoLock(elementStruct->structLock);

			const auto* variable = elementStruct->GetVariable(a_variableName);
			if (!variable) {
				return false;
			}

			return variable->Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (varIt != varEnd) {
			return static_cast<std::int32_t>(std::distance(a_structs.cbegin(), varIt));
		}

		return -1;
	}

	static std::int32_t LastIndexOfValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_structs,
		RE::BSFixedString a_variableName,
		RE::BSScript::SharedVariant a_value,
		std::int32_t a_startIndex,
		std::int32_t a_count)
	{
		if (!a_structs) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ARRAY_NULL, a_stackId);
			return -1;
		}

		const auto size = static_cast<std::int32_t>(a_structs.size());
		if (a_startIndex < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::START_INDEX_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COUNT_NEGATIVE, a_stackId);
			return -1;
		}

		if (a_count == 0 || size == 0) {
			return -1;
		}

		if (a_startIndex >= size) {
			a_startIndex = size - 1;
		}

		a_count = std::min(a_count, size);

		const auto varBegin = a_structs.rbegin() + size - a_startIndex - 1;
		const auto varEnd = a_structs.rbegin() + std::min(size, size - a_startIndex - 1 + a_count);

		const auto varIt = std::find_if(varBegin, varEnd, [&a_variableName, &a_value](const RE::BSScript::Variable& a_element) {
			if (!a_element.IsVar()) {
				return false;
			}

			const auto* elementVar = a_element.GetVar();
			if (!elementVar || !elementVar->IsStruct()) {
				return false;
			}

			const auto elementStruct = elementVar->GetStruct();
			if (!elementStruct) {
				return false;
			}

			const auto structLock = RE::BSAutoLock(elementStruct->structLock);

			const auto* variable = elementStruct->GetVariable(a_variableName);
			if (!variable) {
				return false;
			}

			return variable->Equals(RE::BSScript::UnwrapVariant(a_value));
		});

		if (varIt != varEnd) {
			return static_cast<std::int32_t>(std::distance(a_structs.begin(), varIt.base()) - 1);
		}

		return -1;
	}

	static RE::BSScript::UniqueVariant MergeValues(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_baseStruct,
		RE::BSScript::SharedVariant a_overrideStruct)
	{
		if (!a_baseStruct || !a_baseStruct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return nullptr;
		}

		if (!a_overrideStruct || !a_overrideStruct->IsStruct()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return nullptr;
		}

		const auto baseStructure = a_baseStruct->GetStruct();
		if (!baseStructure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return nullptr;
		}

		const auto overrideStructure = a_overrideStruct->GetStruct();
		if (!overrideStructure) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return nullptr;
		}

		const auto leftTypeInfo = baseStructure->GetTypeInfo();
		if (!leftTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_01_NULL, a_stackId);
			return nullptr;
		}

		const auto rightTypeInfo = overrideStructure->GetTypeInfo();
		if (!rightTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_02_NULL, a_stackId);
			return nullptr;
		}

		if (leftTypeInfo != rightTypeInfo) [[unlikely]] {
			a_vm.PostError("Both structs must be of the same type for merging"sv, a_stackId);
			return nullptr;
		}

		auto mergedStructure = baseStructure->Clone(a_vm);
		if (!mergedStructure) {
			return nullptr;
		}

		if (baseStructure == overrideStructure) {
			return RE::BSScript::CreateVariant(std::move(mergedStructure));
		}

		const auto mergedStructLock = RE::BSAutoLock(mergedStructure->structLock);
		const auto overrideStructLock = RE::BSAutoLock(overrideStructure->structLock);

		const auto structSize = leftTypeInfo->variables.size();

		for (auto i = 0ui32; i < structSize; i++) {
			auto& overrideVar = overrideStructure->variables[i];
			if (overrideVar.IsNone()) {
				continue;
			}

			auto& mergedVar = mergedStructure->variables[i];
			mergedVar.TryAssign(overrideVar);
		}

		return RE::BSScript::CreateVariant(std::move(mergedStructure));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Create);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCompare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCopy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsInstanceOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetStructName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParentScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVariables);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVariableValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVariableValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValuesByName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IndexOfValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, LastIndexOfValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, MergeValues);
	}
}
