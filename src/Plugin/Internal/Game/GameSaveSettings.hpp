#pragma once

namespace Plugin::Internal::Game
{
	class GameIniSettings;

	class GameSaveSettings final
	{
	public:
		GameSaveSettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings);
		~GameSaveSettings() noexcept;

		GameSaveSettings(const GameSaveSettings&) = delete;
		GameSaveSettings(GameSaveSettings&&) = delete;

		GameSaveSettings& operator=(const GameSaveSettings&) = delete;
		GameSaveSettings& operator=(GameSaveSettings&&) = delete;

		void OnGameDataReady();

		[[nodiscard]] REX::Float32 GetLoadInFadeDuration() const;
		void SetLoadInFadeDuration(REX::Float32 a_value);

		[[nodiscard]] bool GetEnableAutoSaves() const;
		void SetEnableAutoSaves(bool a_value);

		[[nodiscard]] bool GetDoSaveOnWorkshopExit() const;
		void SetDoSaveOnWorkshopExit(bool a_value);

		[[nodiscard]] bool GetDoSaveOnFastTravel() const;
		void SetDoSaveOnFastTravel(bool a_value);

		[[nodiscard]] bool GetDoSaveOnRest() const;
		void SetDoSaveOnRest(bool a_value);

		[[nodiscard]] bool GetDoSaveOnWait() const;
		void SetDoSaveOnWait(bool a_value);

		[[nodiscard]] std::int32_t GetMaxAutoSaveCount() const;
		void SetMaxAutoSaveCount(std::int32_t a_value);

	private:
		REX::NotNull<std::shared_ptr<GameIniSettings>> _iniSettings;
		REX::Observer<RE::Setting*> _loadInFadeDurationSetting{ nullptr };
		REX::Observer<RE::Setting*> _disableAutoSaves{ nullptr };
		REX::Observer<RE::Setting*> _doSaveOnWorkshopExitSetting{ nullptr };
		REX::Observer<RE::Setting*> _doSaveOnFastTravelSetting{ nullptr };
		REX::Observer<RE::Setting*> _doSaveOnRestSetting{ nullptr };
		REX::Observer<RE::Setting*> _doSaveOnWaitSetting{ nullptr };
		REX::Observer<RE::Setting*> _maxAutoSaveCountSetting{ nullptr };
	};
}
