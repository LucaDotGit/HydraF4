#pragma once

namespace Plugin::Internal::Game
{
	class GameIniSettings;

	class GameUIColors final
	{
	public:
		class RgbData
		{
		public:
			std::uint8_t red;
			std::uint8_t green;
			std::uint8_t blue;
		};

		GameUIColors(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings);
		~GameUIColors() noexcept;

		GameUIColors(const GameUIColors&) = delete;
		GameUIColors(GameUIColors&&) = delete;

		GameUIColors& operator=(const GameUIColors&) = delete;
		GameUIColors& operator=(GameUIColors&&) = delete;

		void OnGameDataReady();

		[[nodiscard]] RgbData GetHudRgb() const;
		void SetHudRgb(RgbData a_rgb);
		void SaveHudColor() const;
		void SaveHudColorAsync() const;

		[[nodiscard]] RgbData GetPipBoyRgb() const;
		void SetPipBoyRgb(RgbData a_rgb);
		void SavePipBoyColor() const;
		void SavePipBoyColorAsync() const;

		[[nodiscard]] RgbData GetPowerArmorRgb() const;
		void SetPowerArmorRgb(RgbData a_rgb);
		void SavePowerArmorColor() const;
		void SavePowerArmorColorAsync() const;

		[[nodiscard]] RgbData GetWorkbenchHighlightRgb() const;
		void SetWorkbenchHighlightRgb(RgbData a_rgb);
		void SaveWorkbenchHighlightColor() const;
		void SaveWorkbenchHighlightColorAsync() const;

		[[nodiscard]] RgbData GetPowerArmorWorkbenchHighlightRgb() const;
		void SetPowerArmorWorkbenchHighlightRgb(RgbData a_rgb);
		void SavePowerArmorWorkbenchHighlightColor() const;
		void SavePowerArmorWorkbenchHighlightColorAsync() const;

		static void UpdateColors();
		static void UpdateColorsAsync();

	private:
		REX::NotNull<std::shared_ptr<GameIniSettings>> _iniSettings;
		REX::Observer<RE::Setting*> _hudColorRedSetting{ nullptr };
		REX::Observer<RE::Setting*> _hudColorGreenSetting{ nullptr };
		REX::Observer<RE::Setting*> _hudColorBlueSetting{ nullptr };
		REX::Observer<RE::Setting*> _pipBoyColorRedSetting{ nullptr };
		REX::Observer<RE::Setting*> _pipBoyColorGreenSetting{ nullptr };
		REX::Observer<RE::Setting*> _pipBoyColorBlueSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorColorRedSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorColorGreenSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorColorBlueSetting{ nullptr };
		REX::Observer<RE::Setting*> _workbenchHighlightColorRedSetting{ nullptr };
		REX::Observer<RE::Setting*> _workbenchHighlightColorGreenSetting{ nullptr };
		REX::Observer<RE::Setting*> _workbenchHighlightColorBlueSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorWorkbenchHighlightColorRedSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorWorkbenchHighlightColorGreenSetting{ nullptr };
		REX::Observer<RE::Setting*> _powerArmorWorkbenchHighlightColorBlueSetting{ nullptr };
	};
}
