#include "Plugin/Internal/Game/GameCameraSettings.hpp"

#include "Plugin/Internal/Game/GameIniSettings.hpp"

namespace Plugin::Internal::Game
{
	static constexpr auto FIRST_PERSON_FOV_SETTING = "Display:fDefault1stPersonFOV"sv;
	static constexpr auto THIRD_PERSON_FOV_SETTING = "Display:fDefaultWorldFOV"sv;
	static constexpr auto THIRD_PERSON_AIM_FOV_SETTING = "Camera:f3rdPersonAimFOV"sv;
	static constexpr auto NEAR_CLIP_DISTANCE_SETTING = "Display:fNearDistance"sv;

	static constexpr auto MIN_FOV = 30.0_f32;
	static constexpr auto MAX_FOV = 160.0_f32;

	static constexpr auto MIN_CLIP_DISTANCE = 0.0_f32;
	static constexpr auto MAX_CLIP_DISTANCE = 30.0_f32;

	GameCameraSettings::GameCameraSettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings)
		: _iniSettings(a_iniSettings)
	{
	}

	GameCameraSettings::~GameCameraSettings() noexcept = default;

	void GameCameraSettings::OnGameDataReady()
	{
		_firstPersonFovSetting = _iniSettings->GetSetting(FIRST_PERSON_FOV_SETTING);
		if (!_firstPersonFovSetting || !_firstPersonFovSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_thirdPersonFovSetting = _iniSettings->GetSetting(THIRD_PERSON_FOV_SETTING);
		if (!_thirdPersonFovSetting || !_thirdPersonFovSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_thirdPersonAimFovSetting = _iniSettings->GetSetting(THIRD_PERSON_AIM_FOV_SETTING);
		if (!_thirdPersonAimFovSetting || !_thirdPersonAimFovSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_nearClipDistanceSetting = _iniSettings->GetSetting(NEAR_CLIP_DISTANCE_SETTING);
		if (!_nearClipDistanceSetting || !_nearClipDistanceSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}
	}

	REX::Float32 GameCameraSettings::GetFirstPersonFov() const
	{
		if (!_firstPersonFovSetting) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return _firstPersonFovSetting->GetFloat();
	}

	void GameCameraSettings::SetFirstPersonFov(REX::Float32 a_value)
	{
		if (!_firstPersonFovSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_firstPersonFovSetting->SetFloat(std::clamp(a_value, MIN_FOV, MAX_FOV));
	}

	REX::Float32 GameCameraSettings::GetThirdPersonFov() const
	{
		if (!_thirdPersonFovSetting) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return _thirdPersonFovSetting->GetFloat();
	}

	void GameCameraSettings::SetThirdPersonFov(REX::Float32 a_value)
	{
		if (!_thirdPersonFovSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_thirdPersonFovSetting->SetFloat(std::clamp(a_value, MIN_FOV, MAX_FOV));
	}

	REX::Float32 GameCameraSettings::GetThirdPersonAimFov() const
	{
		if (!_thirdPersonAimFovSetting) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return _thirdPersonAimFovSetting->GetFloat();
	}

	void GameCameraSettings::SetThirdPersonAimFov(REX::Float32 a_value)
	{
		if (!_thirdPersonAimFovSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_thirdPersonAimFovSetting->SetFloat(std::clamp(a_value, MIN_FOV, MAX_FOV));
	}

	REX::Float32 GameCameraSettings::GetViewmodelFov()
	{
		const auto* camera = RE::PlayerCamera::GetSingleton();
		if (!camera) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return camera->firstPersonFOV;
	}

	void GameCameraSettings::SetViewmodelFov(REX::Float32 a_value)
	{
		auto* camera = RE::PlayerCamera::GetSingleton();
		if (!camera) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		camera->firstPersonFOV = std::clamp(a_value, MIN_FOV, MAX_FOV);
	}

	REX::Float32 GameCameraSettings::GetNearClipDistance() const
	{
		if (!_nearClipDistanceSetting) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return _nearClipDistanceSetting->GetFloat();
	}

	void GameCameraSettings::SetNearClipDistance(REX::Float32 a_value)
	{
		if (!_nearClipDistanceSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_nearClipDistanceSetting->SetFloat(std::clamp(a_value, MIN_CLIP_DISTANCE, MAX_CLIP_DISTANCE));
	}
}
