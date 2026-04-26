#include "Plugin/Papyrus/Timer.hpp"

#include "Plugin/Internal/Functors/TimerDelayFunctor.hpp"
#include "Plugin/Internal/Functors/TimerDelayFunctorManager.hpp"
#include "Plugin/Internal/Functors/WaitForNextFrameDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Structs/FunctionRefs.hpp"

namespace Plugin::Papyrus::Timer
{
	using FunctionRefStruct = ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData;

	using Seconds = ::Plugin::Internal::Functors::TimerDelayFunctor::Seconds;
	using SecondsUnderlying = Seconds::rep;

	using TimerFlags = ::Plugin::Internal::Functors::TimerDelayFunctor::TimerFlags;

	static bool IsExistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsExistent(scriptFunctionRef);
	}

	static bool IsPersistent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsPersistent(scriptFunctionRef);
	}

	static bool IsRepeating(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsRepeating(scriptFunctionRef);
	}

	static bool IsActive(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsActive(scriptFunctionRef);
	}

	static bool IsPaused(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsPaused(scriptFunctionRef);
	}

	static bool IsRunningInRealTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsRunningInRealTime(scriptFunctionRef);
	}

	static bool IsRunningInGameMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsRunningInGameMode(scriptFunctionRef);
	}

	static bool IsRunningInMenuMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsRunningInMenuMode(scriptFunctionRef);
	}

	static bool IsRunningInGameTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->IsRunningInGameTime(scriptFunctionRef);
	}

	static bool Pause(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->Pause(scriptFunctionRef);
	}

	static bool Resume(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->Resume(scriptFunctionRef);
	}

	static bool Stop(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return false;
		}

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->Stop(scriptFunctionRef);
	}

	static bool StopAllLocal(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StopAllLocal(a_object);
	}

	static bool StopAllGlobal(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_scriptName)
	{
		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StopAllGlobal(a_scriptName);
	}

	static SecondsUnderlying GetIntervalSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return 0;
		}

		const auto initialSeconds = ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->GetIntervalSeconds(scriptFunctionRef);
		return initialSeconds.count();
	}

	static SecondsUnderlying GetElapsedSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return 0;
		}

		const auto intervalSeconds = ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->GetElapsedSeconds(scriptFunctionRef);
		return intervalSeconds.count();
	}

	static SecondsUnderlying GetRemainingSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return 0;
		}

		const auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return 0;
		}

		const auto remainingSeconds = ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->GetRemainingSeconds(scriptFunctionRef);
		return remainingSeconds.count();
	}

	static bool StartRealTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartRealTime(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartGameMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartGameMode(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartMenuMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartMenuMode(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartGameTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartGameTime(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartRepeatingRealTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartRepeatingRealTime(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartRepeatingGameMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartRepeatingGameMode(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartRepeatingMenuMode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartRepeatingMenuMode(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static bool StartRepeatingGameTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		SecondsUnderlying a_delaySeconds,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args,
		bool a_persistent)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
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

		return ::Plugin::Internal::Functors::GetTimerDelayFunctorManager()->StartRepeatingGameTime(
			scriptFunctionRef, static_cast<Seconds>(a_delaySeconds), a_args, a_persistent);
	}

	static RE::BSScript::LatentResult<void> WaitForNextFrame(RE::BSScript::IVirtualMachine& /*a_vm*/, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto waitFunctor = ::Plugin::Internal::Functors::WaitForNextFrameDelayFunctorFactory::GetSingleton()->CreateObject(a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(waitFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return RE::BSScript::LatentStatus::kStarted;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsExistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPersistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRepeating);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsActive);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPaused);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRunningInRealTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRunningInGameMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRunningInMenuMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRunningInGameTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Pause);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Resume);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Stop);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StopAllLocal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StopAllGlobal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIntervalSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetElapsedSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRemainingSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartRealTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartGameMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartMenuMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartGameTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartRepeatingRealTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartRepeatingGameMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartRepeatingMenuMode);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, StartRepeatingGameTime);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, WaitForNextFrame);
	}
}
