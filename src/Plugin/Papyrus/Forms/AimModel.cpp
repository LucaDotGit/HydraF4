#include "Plugin/Papyrus/Forms/AimModel.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::AimModel
{
	static REX::Float32 GetConeMinAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.minConeDegrees).load(std::memory_order_acquire);
	}

	static void SetConeMinAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.minConeDegrees).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetConeMaxAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.maxConeDegrees).load(std::memory_order_acquire);
	}

	static void SetConeMaxAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.maxConeDegrees).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetConeIncreasePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.coneIncreasePerShot).load(std::memory_order_acquire);
	}

	static void SetConeIncreasePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.coneIncreasePerShot).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetConeDecreasePerSec(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.coneDecreasePerSec).load(std::memory_order_acquire);
	}

	static void SetConeDecreasePerSec(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.coneDecreasePerSec).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetConeDecreaseDelayMS(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_aimModel->data.coneDecreaseDelayMs).load(std::memory_order_acquire);
	}

	static void SetConeDecreaseDelayMS(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		std::uint32_t a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.coneDecreaseDelayMs).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetConeSneakMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.coneSneakMultiplier).load(std::memory_order_acquire);
	}

	static void SetConeSneakMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.coneSneakMultiplier).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilArcAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilArcDeg).load(std::memory_order_acquire);
	}

	static void SetRecoilArcAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilArcDeg).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilArcRotateAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilArcRotateDeg).load(std::memory_order_acquire);
	}

	static void SetRecoilArcRotateAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilArcRotateDeg).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilMaxAnglePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilMaxDegPerShot).load(std::memory_order_acquire);
	}

	static void SetRecoilMaxAnglePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilMaxDegPerShot).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilMinAnglePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilMinDegPerShot).load(std::memory_order_acquire);
	}

	static void SetRecoilMinAnglePerShot(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilMinDegPerShot).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilDiminishSpringForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilDiminishSpringForce).load(std::memory_order_acquire);
	}

	static void SetRecoilDiminishSpringForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilDiminishSpringForce).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilDiminishSightsMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilDiminishSightsMult).load(std::memory_order_acquire);
	}

	static void SetRecoilDiminishSightsMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilDiminishSightsMult).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRecoilHipMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.recoilHipMult).load(std::memory_order_acquire);
	}

	static void SetRecoilHipMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilHipMult).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetRunawayRecoilShots(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_aimModel->data.recoilShotsForRunaway).load(std::memory_order_acquire);
	}

	static void SetRunawayRecoilShots(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		std::uint32_t a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.recoilShotsForRunaway).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetBaseStability(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_aimModel->data.baseStability).load(std::memory_order_acquire);
	}

	static void SetBaseStability(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSAimModel* a_aimModel,
		REX::Float32 a_value)
	{
		if (!a_aimModel) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AIM_MODEL_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_aimModel->data.baseStability).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeMinAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeMinAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeMaxAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeMaxAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeIncreasePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeIncreasePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeDecreasePerSec);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeDecreasePerSec);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeDecreaseDelayMS);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeDecreaseDelayMS);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeSneakMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeSneakMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilArcAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilArcAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilArcRotateAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilArcRotateAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilMaxAnglePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilMaxAnglePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilMinAnglePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilMinAnglePerShot);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilDiminishSpringForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilDiminishSpringForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilDiminishSightsMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilDiminishSightsMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecoilHipMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRecoilHipMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRunawayRecoilShots);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRunawayRecoilShots);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBaseStability);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBaseStability);
	}
}
