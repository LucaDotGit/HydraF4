#include "Plugin/Papyrus/ScriptStack.hpp"

namespace Plugin::Papyrus::ScriptStack
{
	static RE::BSScript::StackID GetCurrentStackID(RE::BSScript::IVirtualMachine& /*a_vm*/, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return a_stackId;
	}

	static RE::BSTSmartPointer<RE::BSScript::Object> GetCurrentObject(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* previousFrame = a_stackFrame.previousFrame;
		if (!previousFrame) {
			return nullptr;
		}

		return previousFrame->GetSelfObject();
	}

	static RE::BSFixedString GetCurrentScriptName(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* currentFrame = a_stackFrame.previousFrame;
		if (!currentFrame) {
			return {};
		}

		const auto& currentTypeInfo = currentFrame->owningObjectType;
		if (!currentTypeInfo) {
			return {};
		}

		return currentTypeInfo->GetName();
	}

	static RE::BSFixedString GetCurrentFunctionName(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* currentFrame = a_stackFrame.previousFrame;
		if (!currentFrame) {
			return {};
		}

		const auto& currentFunction = currentFrame->owningFunction;
		if (!currentFunction) {
			return {};
		}

		return currentFunction->GetName();
	}

	static RE::BSFixedString GetPreviousScriptName(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* currentFrame = a_stackFrame.previousFrame;
		if (!currentFrame) {
			return {};
		}

		const auto* previousFrame = currentFrame->previousFrame;
		if (!previousFrame) {
			return {};
		}

		const auto& previousTypeInfo = previousFrame->owningObjectType;
		if (!previousTypeInfo) {
			return {};
		}

		return previousTypeInfo->GetName();
	}

	static RE::BSFixedString GetPreviousFunctionName(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* currentFrame = a_stackFrame.previousFrame;
		if (!currentFrame) {
			return {};
		}

		const auto* previousFrame = currentFrame->previousFrame;
		if (!previousFrame) {
			return {};
		}

		const auto& previousFunction = previousFrame->owningFunction;
		if (!previousFunction) {
			return {};
		}

		return previousFunction->GetName();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentStackID);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentFunctionName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPreviousScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPreviousFunctionName);
	}
}
