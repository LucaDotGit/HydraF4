#pragma once

namespace Plugin::Internal::Game
{
	class GameIniSettings;

	class GameCameraSettings final
	{
	public:
		GameCameraSettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings);
		~GameCameraSettings() noexcept;

		GameCameraSettings(const GameCameraSettings&) = delete;
		GameCameraSettings(GameCameraSettings&&) = delete;

		GameCameraSettings& operator=(const GameCameraSettings&) = delete;
		GameCameraSettings& operator=(GameCameraSettings&&) = delete;

		void OnGameDataReady();

		[[nodiscard]] REX::Float32 GetFirstPersonFov() const;
		void SetFirstPersonFov(REX::Float32 a_value);

		[[nodiscard]] REX::Float32 GetThirdPersonFov() const;
		void SetThirdPersonFov(REX::Float32 a_value);

		[[nodiscard]] REX::Float32 GetThirdPersonAimFov() const;
		void SetThirdPersonAimFov(REX::Float32 a_value);

		[[nodiscard]] static REX::Float32 GetViewmodelFov();
		static void SetViewmodelFov(REX::Float32 a_value);

		[[nodiscard]] REX::Float32 GetNearClipDistance() const;
		void SetNearClipDistance(REX::Float32 a_value);

	private:
		REX::NotNull<std::shared_ptr<GameIniSettings>> _iniSettings;
		REX::Observer<RE::Setting*> _firstPersonFovSetting{ nullptr };
		REX::Observer<RE::Setting*> _thirdPersonFovSetting{ nullptr };
		REX::Observer<RE::Setting*> _thirdPersonAimFovSetting{ nullptr };
		REX::Observer<RE::Setting*> _nearClipDistanceSetting{ nullptr };
	};
}
