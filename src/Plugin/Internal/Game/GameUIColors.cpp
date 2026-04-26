#include "Plugin/Internal/Game/GameUIColors.hpp"

#include "Plugin/Internal/Game/GameIniSettings.hpp"

namespace Plugin::Internal::Game::Impl
{
	static constexpr auto FLOAT_COLOR_DIVISOR = static_cast<REX::Float32>(std::numeric_limits<std::uint8_t>::max());

	[[nodiscard]] static GameUIColors::RgbData GetIntRgb(
		const REX::Observer<RE::Setting*> a_redSetting,
		const REX::Observer<RE::Setting*> a_greenSetting,
		const REX::Observer<RE::Setting*> a_blueSetting)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return { .red = 0, .green = 0, .blue = 0 };
		}

		return {
			.red = static_cast<std::uint8_t>(a_redSetting->GetInt()),
			.green = static_cast<std::uint8_t>(a_greenSetting->GetInt()),
			.blue = static_cast<std::uint8_t>(a_blueSetting->GetInt())
		};
	}

	static void SetIntRgb(
		REX::Observer<RE::Setting*> a_redSetting,
		REX::Observer<RE::Setting*> a_greenSetting,
		REX::Observer<RE::Setting*> a_blueSetting,
		GameUIColors::RgbData a_rgb)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		a_redSetting->SetInt(a_rgb.red);
		a_greenSetting->SetInt(a_rgb.green);
		a_blueSetting->SetInt(a_rgb.blue);
	}

	[[nodiscard]] static GameUIColors::RgbData GetFloatRgb(
		const REX::Observer<RE::Setting*> a_redSetting,
		const REX::Observer<RE::Setting*> a_greenSetting,
		const REX::Observer<RE::Setting*> a_blueSetting)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return { .red = 0, .green = 0, .blue = 0 };
		}

		return {
			.red = static_cast<std::uint8_t>(a_redSetting->GetFloat() * FLOAT_COLOR_DIVISOR),
			.green = static_cast<std::uint8_t>(a_greenSetting->GetFloat() * FLOAT_COLOR_DIVISOR),
			.blue = static_cast<std::uint8_t>(a_blueSetting->GetFloat() * FLOAT_COLOR_DIVISOR)
		};
	}

	static void SetFloatRgb(
		REX::Observer<RE::Setting*> a_redSetting,
		REX::Observer<RE::Setting*> a_greenSetting,
		REX::Observer<RE::Setting*> a_blueSetting,
		GameUIColors::RgbData a_rgb)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		a_redSetting->SetFloat(static_cast<REX::Float32>(a_rgb.red) / FLOAT_COLOR_DIVISOR);
		a_greenSetting->SetFloat(static_cast<REX::Float32>(a_rgb.green) / FLOAT_COLOR_DIVISOR);
		a_blueSetting->SetFloat(static_cast<REX::Float32>(a_rgb.blue) / FLOAT_COLOR_DIVISOR);
	}

	static void SaveSetting(
		REX::Observer<RE::Setting*> a_redSetting,
		REX::Observer<RE::Setting*> a_greenSetting,
		REX::Observer<RE::Setting*> a_blueSetting)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		if (!GameIniSettings::SaveSetting(a_redSetting)) [[unlikely]] {
			REX::Assert(false);
		}

		if (!GameIniSettings::SaveSetting(a_greenSetting)) [[unlikely]] {
			REX::Assert(false);
		}

		if (!GameIniSettings::SaveSetting(a_blueSetting)) [[unlikely]] {
			REX::Assert(false);
		}
	}

	static void SaveSettingAsync(
		REX::Observer<RE::Setting*> a_redSetting,
		REX::Observer<RE::Setting*> a_greenSetting,
		REX::Observer<RE::Setting*> a_blueSetting)
	{
		if (!a_redSetting || !a_greenSetting || !a_blueSetting) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([red = a_redSetting, green = a_greenSetting, blue = a_blueSetting]() {
			SaveSetting(red, green, blue);
		});
	}
}

namespace Plugin::Internal::Game
{
	static constexpr auto HUD_COLOR_R_SETTING = "Interface:iHUDColorR"sv;
	static constexpr auto HUD_COLOR_G_SETTING = "Interface:iHUDColorG"sv;
	static constexpr auto HUD_COLOR_B_SETTING = "Interface:iHUDColorB"sv;

	static constexpr auto PIP_BOY_COLOR_R_SETTING = "Pipboy:fPipboyEffectColorR"sv;
	static constexpr auto PIP_BOY_COLOR_G_SETTING = "Pipboy:fPipboyEffectColorG"sv;
	static constexpr auto PIP_BOY_COLOR_B_SETTING = "Pipboy:fPipboyEffectColorB"sv;

	static constexpr auto POWER_ARMOR_COLOR_R_SETTING = "Pipboy:fPAEffectColorR"sv;
	static constexpr auto POWER_ARMOR_COLOR_G_SETTING = "Pipboy:fPAEffectColorG"sv;
	static constexpr auto POWER_ARMOR_COLOR_B_SETTING = "Pipboy:fPAEffectColorB"sv;

	static constexpr auto WORKBENCH_HIGHLIGHT_COLOR_R_SETTING = "VATS:fModMenuEffectHighlightColorR"sv;
	static constexpr auto WORKBENCH_HIGHLIGHT_COLOR_G_SETTING = "VATS:fModMenuEffectHighlightColorG"sv;
	static constexpr auto WORKBENCH_HIGHLIGHT_COLOR_B_SETTING = "VATS:fModMenuEffectHighlightColorB"sv;

	static constexpr auto POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_R_SETTING = "VATS:fModMenuEffectHighlightPAColorR"sv;
	static constexpr auto POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_G_SETTING = "VATS:fModMenuEffectHighlightPAColorG"sv;
	static constexpr auto POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_B_SETTING = "VATS:fModMenuEffectHighlightPAColorB"sv;

	GameUIColors::GameUIColors(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings)
		: _iniSettings(a_iniSettings)
	{
	}

	GameUIColors::~GameUIColors() noexcept = default;

	void GameUIColors::OnGameDataReady()
	{
		_hudColorRedSetting = _iniSettings->GetSetting(HUD_COLOR_R_SETTING);
		if (!_hudColorRedSetting || !_hudColorRedSetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
		}

		_hudColorGreenSetting = _iniSettings->GetSetting(HUD_COLOR_G_SETTING);
		if (!_hudColorGreenSetting || !_hudColorGreenSetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
		}

		_hudColorBlueSetting = _iniSettings->GetSetting(HUD_COLOR_B_SETTING);
		if (!_hudColorBlueSetting || !_hudColorBlueSetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
		}

		_pipBoyColorRedSetting = _iniSettings->GetSetting(PIP_BOY_COLOR_R_SETTING);
		if (!_pipBoyColorRedSetting || !_pipBoyColorRedSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_pipBoyColorGreenSetting = _iniSettings->GetSetting(PIP_BOY_COLOR_G_SETTING);
		if (!_pipBoyColorGreenSetting || !_pipBoyColorGreenSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_pipBoyColorBlueSetting = _iniSettings->GetSetting(PIP_BOY_COLOR_B_SETTING);
		if (!_pipBoyColorBlueSetting || !_pipBoyColorBlueSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorColorRedSetting = _iniSettings->GetSetting(POWER_ARMOR_COLOR_R_SETTING);
		if (!_powerArmorColorRedSetting || !_powerArmorColorRedSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorColorGreenSetting = _iniSettings->GetSetting(POWER_ARMOR_COLOR_G_SETTING);
		if (!_powerArmorColorGreenSetting || !_powerArmorColorGreenSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorColorBlueSetting = _iniSettings->GetSetting(POWER_ARMOR_COLOR_B_SETTING);
		if (!_powerArmorColorBlueSetting || !_powerArmorColorBlueSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_workbenchHighlightColorRedSetting = _iniSettings->GetSetting(WORKBENCH_HIGHLIGHT_COLOR_R_SETTING);
		if (!_workbenchHighlightColorRedSetting || !_workbenchHighlightColorRedSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_workbenchHighlightColorGreenSetting = _iniSettings->GetSetting(WORKBENCH_HIGHLIGHT_COLOR_G_SETTING);
		if (!_workbenchHighlightColorGreenSetting || !_workbenchHighlightColorGreenSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_workbenchHighlightColorBlueSetting = _iniSettings->GetSetting(WORKBENCH_HIGHLIGHT_COLOR_B_SETTING);
		if (!_workbenchHighlightColorBlueSetting || !_workbenchHighlightColorBlueSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorWorkbenchHighlightColorRedSetting = _iniSettings->GetSetting(POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_R_SETTING);
		if (!_powerArmorWorkbenchHighlightColorRedSetting || !_powerArmorWorkbenchHighlightColorRedSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorWorkbenchHighlightColorGreenSetting = _iniSettings->GetSetting(POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_G_SETTING);
		if (!_powerArmorWorkbenchHighlightColorGreenSetting || !_powerArmorWorkbenchHighlightColorGreenSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}

		_powerArmorWorkbenchHighlightColorBlueSetting = _iniSettings->GetSetting(POWER_ARMOR_WORKBENCH_HIGHLIGHT_COLOR_B_SETTING);
		if (!_powerArmorWorkbenchHighlightColorBlueSetting || !_powerArmorWorkbenchHighlightColorBlueSetting->IsFloat()) [[unlikely]] {
			REX::Assert(false);
		}
	}

	GameUIColors::RgbData GameUIColors::GetHudRgb() const
	{
		return Impl::GetIntRgb(
			_hudColorRedSetting,
			_hudColorGreenSetting,
			_hudColorBlueSetting);
	}

	void GameUIColors::SetHudRgb(GameUIColors::RgbData a_rgb)
	{
		Impl::SetIntRgb(
			_hudColorRedSetting,
			_hudColorGreenSetting,
			_hudColorBlueSetting,
			a_rgb);

		if (!RE::PowerArmor::PlayerInPowerArmor()) {
			UpdateColorsAsync();
		}
	}

	void GameUIColors::SaveHudColor() const
	{
		Impl::SaveSetting(
			_hudColorRedSetting,
			_hudColorGreenSetting,
			_hudColorBlueSetting);
	}

	void GameUIColors::SaveHudColorAsync() const
	{
		Impl::SaveSettingAsync(
			_hudColorRedSetting,
			_hudColorGreenSetting,
			_hudColorBlueSetting);
	}

	GameUIColors::RgbData GameUIColors::GetPipBoyRgb() const
	{
		return Impl::GetFloatRgb(
			_pipBoyColorRedSetting,
			_pipBoyColorGreenSetting,
			_pipBoyColorBlueSetting);
	}

	void GameUIColors::SetPipBoyRgb(GameUIColors::RgbData a_rgb)
	{
		Impl::SetFloatRgb(
			_pipBoyColorRedSetting,
			_pipBoyColorGreenSetting,
			_pipBoyColorBlueSetting,
			a_rgb);
	}

	void GameUIColors::SavePipBoyColor() const
	{
		Impl::SaveSetting(
			_pipBoyColorRedSetting,
			_pipBoyColorGreenSetting,
			_pipBoyColorBlueSetting);
	}

	void GameUIColors::SavePipBoyColorAsync() const
	{
		Impl::SaveSettingAsync(
			_pipBoyColorRedSetting,
			_pipBoyColorGreenSetting,
			_pipBoyColorBlueSetting);
	}

	GameUIColors::RgbData GameUIColors::GetPowerArmorRgb() const
	{
		return Impl::GetFloatRgb(
			_powerArmorColorRedSetting,
			_powerArmorColorGreenSetting,
			_powerArmorColorBlueSetting);
	}

	void GameUIColors::SetPowerArmorRgb(GameUIColors::RgbData a_rgb)
	{
		Impl::SetFloatRgb(
			_powerArmorColorRedSetting,
			_powerArmorColorGreenSetting,
			_powerArmorColorBlueSetting,
			a_rgb);

		if (RE::PowerArmor::PlayerInPowerArmor()) {
			UpdateColorsAsync();
		}
	}

	void GameUIColors::SavePowerArmorColor() const
	{
		Impl::SaveSetting(
			_powerArmorColorRedSetting,
			_powerArmorColorGreenSetting,
			_powerArmorColorBlueSetting);
	}

	void GameUIColors::SavePowerArmorColorAsync() const
	{
		Impl::SaveSettingAsync(
			_powerArmorColorRedSetting,
			_powerArmorColorGreenSetting,
			_powerArmorColorBlueSetting);
	}

	GameUIColors::RgbData GameUIColors::GetWorkbenchHighlightRgb() const
	{
		return Impl::GetFloatRgb(
			_workbenchHighlightColorRedSetting,
			_workbenchHighlightColorGreenSetting,
			_workbenchHighlightColorBlueSetting);
	}

	void GameUIColors::SetWorkbenchHighlightRgb(GameUIColors::RgbData a_rgb)
	{
		Impl::SetFloatRgb(
			_workbenchHighlightColorRedSetting,
			_workbenchHighlightColorGreenSetting,
			_workbenchHighlightColorBlueSetting,
			a_rgb);
	}

	void GameUIColors::SaveWorkbenchHighlightColor() const
	{
		Impl::SaveSetting(
			_workbenchHighlightColorRedSetting,
			_workbenchHighlightColorGreenSetting,
			_workbenchHighlightColorBlueSetting);
	}

	void GameUIColors::SaveWorkbenchHighlightColorAsync() const
	{
		Impl::SaveSettingAsync(
			_workbenchHighlightColorRedSetting,
			_workbenchHighlightColorGreenSetting,
			_workbenchHighlightColorBlueSetting);
	}

	GameUIColors::RgbData GameUIColors::GetPowerArmorWorkbenchHighlightRgb() const
	{
		return Impl::GetFloatRgb(
			_powerArmorWorkbenchHighlightColorRedSetting,
			_powerArmorWorkbenchHighlightColorGreenSetting,
			_powerArmorWorkbenchHighlightColorBlueSetting);
	}

	void GameUIColors::SetPowerArmorWorkbenchHighlightRgb(GameUIColors::RgbData a_rgb)
	{
		Impl::SetFloatRgb(
			_powerArmorWorkbenchHighlightColorRedSetting,
			_powerArmorWorkbenchHighlightColorGreenSetting,
			_powerArmorWorkbenchHighlightColorBlueSetting,
			a_rgb);
	}

	void GameUIColors::SavePowerArmorWorkbenchHighlightColor() const
	{
		Impl::SaveSetting(
			_powerArmorWorkbenchHighlightColorRedSetting,
			_powerArmorWorkbenchHighlightColorGreenSetting,
			_powerArmorWorkbenchHighlightColorBlueSetting);
	}

	void GameUIColors::SavePowerArmorWorkbenchHighlightColorAsync() const
	{
		Impl::SaveSettingAsync(
			_powerArmorWorkbenchHighlightColorRedSetting,
			_powerArmorWorkbenchHighlightColorGreenSetting,
			_powerArmorWorkbenchHighlightColorBlueSetting);
	}

	void GameUIColors::UpdateColors()
	{
		const auto colorUpdateEvent = RE::ColorUpdateEvent();
		RE::ColorUpdateEvent::GetEventSource()->Notify(colorUpdateEvent);
	}

	void GameUIColors::UpdateColorsAsync()
	{
		F4SE::GetTaskInterface()->AddUITask([]() {
			UpdateColors();
		});
	}
}
