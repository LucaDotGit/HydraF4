#include "Plugin/Papyrus/Camera.hpp"

#include "Plugin/Internal/Game/GameCameraSettings.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::Camera
{
	using CameraStates = RE::CameraStates;

	static CameraStates GetCameraState(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		constexpr auto DEFAULT_CAMERA_STATE = static_cast<CameraStates>(-1);

		const auto* camera = RE::PlayerCamera::GetSingleton();
		if (!camera) [[unlikely]] {
			REX::Assert(false);
			return DEFAULT_CAMERA_STATE;
		}

		const auto state = camera->GetState();
		if (!state) {
			return DEFAULT_CAMERA_STATE;
		}

		return state->id.get_atomic();
	}

	static bool SetCameraState(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		CameraStates a_value)
	{
		constexpr auto MIN_CAMERA_STATE = CameraStates::kMin;
		constexpr auto MAX_CAMERA_STATE = CameraStates::kMax;

		if (a_value < MIN_CAMERA_STATE || a_value > MAX_CAMERA_STATE) [[unlikely]] {
			a_vm.PostError("The specified camera state {} is invalid ({} - {})"sv, a_stackId,
				std::to_underlying(a_value), std::to_underlying(MIN_CAMERA_STATE), std::to_underlying(MAX_CAMERA_STATE));
			return false;
		}

		auto* playerCamera = RE::PlayerCamera::GetSingleton();
		if (!playerCamera) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto newState = playerCamera->GetState(a_value);
		if (playerCamera->IsStateActive(newState)) {
			return false;
		}

		playerCamera->SetState(newState);
		return true;
	}

	static CameraStates GetCameraState_FirstPerson(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kFirstPerson; }
	static CameraStates GetCameraState_AutoVanity(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kAutoVanity; }
	static CameraStates GetCameraState_Vats(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kVATS; }
	static CameraStates GetCameraState_FreeCamera(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kFree; }
	static CameraStates GetCameraState_IronSights(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kIronSights; }
	static CameraStates GetCameraState_Transition(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kPCTransition; }
	static CameraStates GetCameraState_Tween(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kTween; }
	static CameraStates GetCameraState_Animated(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kAnimated; }
	static CameraStates GetCameraState_ThirdPerson(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kThirdPerson; }
	static CameraStates GetCameraState_Furniture(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kFurniture; }
	static CameraStates GetCameraState_Mount(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kMount; }
	static CameraStates GetCameraState_Bleedout(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kBleedout; }
	static CameraStates GetCameraState_Dialogue(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CameraStates::kDialogue; }

	static REX::Float32 GetFirstPersonFov(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->GetFirstPersonFov();
	}

	static void SetFirstPersonFov(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->SetFirstPersonFov(a_value);
	}

	static REX::Float32 GetThirdPersonFov(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->GetThirdPersonFov();
	}

	static void SetThirdPersonFov(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->SetThirdPersonFov(a_value);
	}

	static REX::Float32 GetThirdPersonAimFov(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->GetThirdPersonAimFov();
	}

	static void SetThirdPersonAimFov(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->SetThirdPersonAimFov(a_value);
	}

	static REX::Float32 GetViewmodelFov(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::Internal::Game::GameCameraSettings::GetViewmodelFov();
	}

	static void SetViewmodelFov(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::Internal::Game::GameCameraSettings::SetViewmodelFov(a_value);
	}

	static REX::Float32 GetNearClipDistance(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->GetNearClipDistance();
	}

	static void SetNearClipDistance(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::Float32 a_value)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetCameraSettings()->SetNearClipDistance(a_value);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetCameraState);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_FirstPerson);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_AutoVanity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Vats);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_FreeCamera);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_IronSights);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Transition);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Tween);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Animated);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_ThirdPerson);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Furniture);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Mount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Bleedout);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCameraState_Dialogue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFirstPersonFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFirstPersonFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetThirdPersonFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetThirdPersonFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetThirdPersonAimFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetThirdPersonAimFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetViewmodelFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetViewmodelFov);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNearClipDistance);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNearClipDistance);
	}
}
