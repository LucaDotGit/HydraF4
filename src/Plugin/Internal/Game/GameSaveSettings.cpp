#include "Plugin/Internal/Game/GameSaveSettings.hpp"

#include "Plugin/Internal/Game/GameIniSettings.hpp"

namespace Plugin::Internal::Game
{
	static constexpr auto LOAD_IN_FADE_DURATION_SETTING = "SaveGame:fSaveGameFadeInDuration"sv;
	static constexpr auto DISABLE_AUTO_SAVES_SETTING = "SaveGame:bDisableAutoSave"sv;
	static constexpr auto DOES_SAVE_ON_WORKSHOP_EXIT_SETTING = "Workshop:bWorkshopAutoSaveOnExit"sv;
	static constexpr auto DOES_SAVE_ON_FAST_TRAVEL_SETTING = "MAIN:bSaveOnTravel"sv;
	static constexpr auto DOES_SAVE_ON_REST_SETTING = "MAIN:bSaveOnRest"sv;
	static constexpr auto DOES_SAVE_ON_WAIT_SETTING = "MAIN:bSaveOnWait"sv;
	static constexpr auto MAX_AUTO_SAVES_SETTING = "SaveGame:iAutoSaveCount"sv;

	static constexpr auto MAX_LOAD_IN_FADE_DURATION = -1.0_f32;
	static constexpr auto MIN_LOAD_IN_FADE_DURATION = -0.0001_f32;

	GameSaveSettings::GameSaveSettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings)
		: _iniSettings(a_iniSettings)
	{
	}

	GameSaveSettings::~GameSaveSettings() noexcept = default;

	void GameSaveSettings::OnGameDataReady()
	{
		_loadInFadeDurationSetting = _iniSettings->GetSetting(LOAD_IN_FADE_DURATION_SETTING);
		if (!_loadInFadeDurationSetting || !_loadInFadeDurationSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_disableAutoSaves = _iniSettings->GetSetting(DISABLE_AUTO_SAVES_SETTING);
		if (!_disableAutoSaves || !_disableAutoSaves->IsBinary()) [[unlikely]] {
			REX::Assert(false);
		}

		_doSaveOnWorkshopExitSetting = _iniSettings->GetSetting(DOES_SAVE_ON_WORKSHOP_EXIT_SETTING);
		if (!_doSaveOnWorkshopExitSetting || !_doSaveOnWorkshopExitSetting->IsBinary()) [[unlikely]] {
			REX::Assert(false);
		}

		_doSaveOnFastTravelSetting = _iniSettings->GetSetting(DOES_SAVE_ON_FAST_TRAVEL_SETTING);
		if (!_doSaveOnFastTravelSetting || !_doSaveOnFastTravelSetting->IsBinary()) [[unlikely]] {
			REX::Assert(false);
		}

		_doSaveOnRestSetting = _iniSettings->GetSetting(DOES_SAVE_ON_REST_SETTING);
		if (!_doSaveOnRestSetting || !_doSaveOnRestSetting->IsBinary()) [[unlikely]] {
			REX::Assert(false);
		}

		_doSaveOnWaitSetting = _iniSettings->GetSetting(DOES_SAVE_ON_WAIT_SETTING);
		if (!_doSaveOnWaitSetting || !_doSaveOnWaitSetting->IsBinary()) [[unlikely]] {
			REX::Assert(false);
		}

		_maxAutoSaveCountSetting = _iniSettings->GetSetting(MAX_AUTO_SAVES_SETTING);
		if (!_maxAutoSaveCountSetting || !_maxAutoSaveCountSetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
		}
	}

	REX::Float32 GameSaveSettings::GetLoadInFadeDuration() const
	{
		if (!_loadInFadeDurationSetting) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return _loadInFadeDurationSetting->GetFloat();
	}

	void GameSaveSettings::SetLoadInFadeDuration(REX::Float32 a_value)
	{
		if (!_loadInFadeDurationSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_loadInFadeDurationSetting->SetFloat(std::clamp(a_value, MIN_LOAD_IN_FADE_DURATION, MAX_LOAD_IN_FADE_DURATION));
	}

	bool GameSaveSettings::GetEnableAutoSaves() const
	{
		if (!_disableAutoSaves) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return !_disableAutoSaves->GetBinary();
	}

	void GameSaveSettings::SetEnableAutoSaves(bool a_value)
	{
		if (!_disableAutoSaves) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_disableAutoSaves->SetBinary(!a_value);
	}

	bool GameSaveSettings::GetDoSaveOnWorkshopExit() const
	{
		if (!_doSaveOnWorkshopExitSetting) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return _doSaveOnWorkshopExitSetting->GetBinary();
	}

	void GameSaveSettings::SetDoSaveOnWorkshopExit(bool a_value)
	{
		if (!_doSaveOnWorkshopExitSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_doSaveOnWorkshopExitSetting->SetBinary(a_value);
	}

	bool GameSaveSettings::GetDoSaveOnFastTravel() const
	{
		if (!_doSaveOnFastTravelSetting) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return _doSaveOnFastTravelSetting->GetBinary();
	}

	void GameSaveSettings::SetDoSaveOnFastTravel(bool a_value)
	{
		if (!_doSaveOnFastTravelSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_doSaveOnFastTravelSetting->SetBinary(a_value);
	}

	bool GameSaveSettings::GetDoSaveOnRest() const
	{
		if (!_doSaveOnRestSetting) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return _doSaveOnRestSetting->GetBinary();
	}

	void GameSaveSettings::SetDoSaveOnRest(bool a_value)
	{
		if (!_doSaveOnRestSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_doSaveOnRestSetting->SetBinary(a_value);
	}

	bool GameSaveSettings::GetDoSaveOnWait() const
	{
		if (!_doSaveOnWaitSetting) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return _doSaveOnWaitSetting->GetBinary();
	}

	void GameSaveSettings::SetDoSaveOnWait(bool a_value)
	{
		if (!_doSaveOnWaitSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_doSaveOnWaitSetting->SetBinary(a_value);
	}

	std::int32_t GameSaveSettings::GetMaxAutoSaveCount() const
	{
		if (!_maxAutoSaveCountSetting) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return _maxAutoSaveCountSetting->GetInt();
	}

	void GameSaveSettings::SetMaxAutoSaveCount(std::int32_t a_value)
	{
		if (!_maxAutoSaveCountSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		_maxAutoSaveCountSetting->SetInt(a_value);
	}
}
