#include "Plugin/Papyrus/FunctionRefs.hpp"

#include "Plugin/Internal/Functors/InvokeFunctionDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Structs/FunctionRefs.hpp"

namespace Plugin::Papyrus::FunctionRefs
{
	using FunctionRefStruct = ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData;

	static bool IsRefExistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return scriptFunctionRef.IsFunctionExistent();
	}

	static bool IsLocalFunctionExistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName)
	{
		return a_vm.GetMemberFunction(a_scriptName, a_functionName) != nullptr;
	}

	static bool IsGlobalFunctionExistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName)
	{
		return a_vm.GetStaticFunction(a_scriptName, a_functionName) != nullptr;
	}

	static bool IsRefInvokableWithArgs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return scriptFunctionRef.IsFunctionOfParamTypes(a_args);
	}

	static bool IsLocalFunctionInvokableWithArgs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		const auto memberFunc = a_vm.GetMemberFunction(a_scriptName, a_functionName);
		if (!memberFunc) {
			return false;
		}

		return memberFunc->HasParamsOfVariadicTypes(a_args);
	}

	static bool IsGlobalFunctionInvokableWithArgs(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		const auto staticFunc = a_vm.GetStaticFunction(a_scriptName, a_functionName);
		if (!staticFunc) {
			return false;
		}

		return staticFunc->HasParamsOfVariadicTypes(a_args);
	}

	static RE::BSScript::LatentResult<RE::BSScript::SharedVariant> InvokeRef(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto invokeFunctor = ::Plugin::Internal::Functors::InvokeFunctionDelayFunctorFactory::GetSingleton()->CreateObject(std::move(scriptFunctionRef), a_stackId, a_args);
		F4SE::GetDelayFunctorManager()->Enqueue(invokeFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);

		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSScript::SharedVariant> InvokeLocalFunction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_functionName);
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto invokeFunctor = ::Plugin::Internal::Functors::InvokeFunctionDelayFunctorFactory::GetSingleton()->CreateObject(std::move(scriptFunctionRef), a_stackId, a_args);
		F4SE::GetDelayFunctorManager()->Enqueue(invokeFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);

		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<RE::BSScript::SharedVariant> InvokeGlobalFunction(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_scriptName, a_functionName);
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto invokeFunctor = ::Plugin::Internal::Functors::InvokeFunctionDelayFunctorFactory::GetSingleton()->CreateObject(std::move(scriptFunctionRef), a_stackId, a_args);
		F4SE::GetDelayFunctorManager()->Enqueue(invokeFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);

		return RE::BSScript::LatentStatus::kStarted;
	}

	static bool InvokeRefAsync(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return false;
		}

		return scriptFunctionRef.InvokeAppliedFunction(a_args);
	}

	static bool InvokeLocalFunctionAsync(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_functionName);
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return false;
		}

		return scriptFunctionRef.InvokeAppliedFunction(a_args);
	}

	static bool InvokeGlobalFunctionAsync(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_scriptName, a_functionName);
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		if (!scriptFunctionRef.IsFunctionOfParamTypes(a_args)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return false;
		}

		return scriptFunctionRef.InvokeAppliedFunction(a_args);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRefExistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLocalFunctionExistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsGlobalFunctionExistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRefInvokableWithArgs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLocalFunctionInvokableWithArgs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsGlobalFunctionInvokableWithArgs);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, InvokeRef);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, InvokeLocalFunction);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, InvokeGlobalFunction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, InvokeRefAsync);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, InvokeLocalFunctionAsync);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, InvokeGlobalFunctionAsync);
	}
}
