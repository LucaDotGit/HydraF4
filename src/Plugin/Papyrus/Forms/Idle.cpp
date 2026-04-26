#include "Plugin/Papyrus/Forms/Idle.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Idle
{
	static RE::BSFixedString GetBehaviorGraphName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return {};
		}

		return a_idle->behaviorGraphName;
	}

	static void SetBehaviorGraphName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		RE::BSFixedString a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		a_idle->behaviorGraphName = std::move(a_value);
	}

	static RE::BSFixedString GetAnimationEventName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return {};
		}

		return a_idle->animEventName;
	}

	static void SetAnimationEventName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		RE::BSFixedString a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		a_idle->animEventName = std::move(a_value);
	}

	static RE::BSFixedString GetAnimationFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return {};
		}

		return a_idle->animFileName;
	}

	static void SetAnimationFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		RE::BSFixedString a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		a_idle->animFileName = std::move(a_value);
	}

	static RE::TESIdleForm* GetParentIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_idle->parentIdle).load(std::memory_order_acquire);
	}

	static void SetParentIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		RE::TESIdleForm* a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_idle->parentIdle).store(a_value, std::memory_order_release);
	}

	static RE::TESIdleForm* GetPreviousIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_idle->previousIdle).load(std::memory_order_acquire);
	}

	static void SetPreviousIdle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		RE::TESIdleForm* a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_idle->previousIdle).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetLoopMinSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_idle->data.loopMin).load(std::memory_order_acquire);
	}

	static void SetLoopMinSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		std::uint8_t a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_idle->data.loopMin).store(a_value, std::memory_order_release);
	}

	static std::uint8_t GetLoopMaxSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_idle->data.loopMax).load(std::memory_order_acquire);
	}

	static void SetLoopMaxSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		std::uint8_t a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_idle->data.loopMax).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetReplayDelay(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_idle->data.replayDelay).load(std::memory_order_acquire);
	}

	static void SetReplayDelay(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESIdleForm* a_idle,
		std::uint16_t a_value)
	{
		if (!a_idle) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::IDLE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_idle->data.replayDelay).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBehaviorGraphName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBehaviorGraphName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAnimationEventName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAnimationEventName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAnimationFilePath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAnimationFilePath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParentIdle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetParentIdle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPreviousIdle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPreviousIdle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLoopMinSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLoopMinSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLoopMaxSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLoopMaxSeconds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetReplayDelay);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetReplayDelay);
	}
}
