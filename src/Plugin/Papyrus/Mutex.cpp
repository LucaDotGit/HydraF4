#include "Plugin/Papyrus/Mutex.hpp"

#include "Plugin/Internal/Functors/MutexDelayFunctorManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Mutex
{
	using LockResult = ::Plugin::Internal::Functors::MutexDelayFunctorManager::LockResult;
}

namespace Plugin::Papyrus::Mutex::Impl
{
	[[nodiscard]] __forceinline static RE::BSScript::LatentResult<void> ReturnLatentFunctionByLockResult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		LockResult a_lockResult)
	{
		switch (a_lockResult) {
			case LockResult::Failure: {
				return RE::BSScript::LatentStatus::kFailed;
			}
			case LockResult::IsOwner: {
				return a_vm.ReturnLatentResult(a_stackId);
			}
			case LockResult::IsWaiter: {
				return RE::BSScript::LatentStatus::kStarted;
			}
			default: {
				std::unreachable();
			}
		}
	}
}

namespace Plugin::Papyrus::Mutex
{
	using Seconds = ::Plugin::Internal::Functors::MutexDelayFunctor::Seconds;
	using SecondsUnderlying = Seconds::rep;

	static RE::BSScript::LatentResult<void> LockCurrent(RE::BSScript::IVirtualMachine& a_vm, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* parentStack = a_stackFrame.parentStack;
		if (!parentStack) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto currentStackId = parentStack->stackID;
		if (currentStackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto* previousStackFrame = a_stackFrame.previousFrame;
		if (!previousStackFrame) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(*previousStackFrame);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Lock(scriptFunctionRef, currentStackId);
		return Impl::ReturnLatentFunctionByLockResult(a_vm, currentStackId, lockResult);
	}

	static RE::BSScript::LatentResult<void> LockLocal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_key)
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

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_key);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Lock(scriptFunctionRef, a_stackId);
		return Impl::ReturnLatentFunctionByLockResult(a_vm, a_stackId, lockResult);
	}

	static RE::BSScript::LatentResult<void> LockGlobal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSFixedString a_key)
	{
		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_namespace, a_key);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Lock(scriptFunctionRef, a_stackId);
		return Impl::ReturnLatentFunctionByLockResult(a_vm, a_stackId, lockResult);
	}

	static RE::BSScript::LatentResult<void> LockCurrentForSeconds(RE::BSScript::IVirtualMachine& a_vm, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/,
		SecondsUnderlying a_timeoutSeconds)
	{
		const auto* parentStack = a_stackFrame.parentStack;
		if (!parentStack) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto currentStackId = parentStack->stackID;
		if (currentStackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto* previousStackFrame = a_stackFrame.previousFrame;
		if (!previousStackFrame) [[unlikely]] {
			REX::Assert(false);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(*previousStackFrame);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->LockForSeconds(
			scriptFunctionRef, currentStackId, static_cast<Seconds>(a_timeoutSeconds));
		return Impl::ReturnLatentFunctionByLockResult(a_vm, currentStackId, lockResult);
	}

	static RE::BSScript::LatentResult<void> LockLocalForSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_key,
		SecondsUnderlying a_timeoutSeconds)
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

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_key);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->LockForSeconds(
			scriptFunctionRef, a_stackId, static_cast<Seconds>(a_timeoutSeconds));
		return Impl::ReturnLatentFunctionByLockResult(a_vm, a_stackId, lockResult);
	}

	static RE::BSScript::LatentResult<void> LockGlobalForSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSFixedString a_key,
		SecondsUnderlying a_timeoutSeconds)
	{
		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_namespace, a_key);
		const auto lockResult = ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->LockForSeconds(
			scriptFunctionRef, a_stackId, static_cast<Seconds>(a_timeoutSeconds));
		return Impl::ReturnLatentFunctionByLockResult(a_vm, a_stackId, lockResult);
	}

	static bool TryLockCurrent(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* parentStack = a_stackFrame.parentStack;
		if (!parentStack) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto currentStackId = parentStack->stackID;
		if (currentStackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto* previousStackFrame = a_stackFrame.previousFrame;
		if (!previousStackFrame) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(*previousStackFrame);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->TryLock(scriptFunctionRef, currentStackId);
	}

	static bool TryLockLocal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_key)
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

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_key);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->TryLock(scriptFunctionRef, a_stackId);
	}

	static bool TryLockGlobal(RE::BSScript::IVirtualMachine& /*a_vm*/, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSFixedString a_key)
	{
		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_namespace, a_key);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->TryLock(scriptFunctionRef, a_stackId);
	}

	static bool UnlockCurrent(RE::BSScript::IVirtualMachine& /*a_vm*/, const RE::BSScript::StackFrame& a_stackFrame, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* previousStackFrame = a_stackFrame.previousFrame;
		if (!previousStackFrame) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(*previousStackFrame);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Unlock(scriptFunctionRef);
	}

	static bool UnlockLocal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_key)
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

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_object->GetHandle(), scriptName.value(), a_key);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Unlock(scriptFunctionRef);
	}

	static bool UnlockGlobal(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_namespace,
		RE::BSFixedString a_key)
	{
		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_namespace, a_key);
		return ::Plugin::Internal::Functors::GetMutexDelayFunctorManager()->Unlock(scriptFunctionRef);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockCurrent);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockLocal);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockGlobal);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockCurrentForSeconds);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockLocalForSeconds);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, LockGlobalForSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TryLockCurrent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TryLockLocal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TryLockGlobal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnlockCurrent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnlockLocal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, UnlockGlobal);
	}
}
