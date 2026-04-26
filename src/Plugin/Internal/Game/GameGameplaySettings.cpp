#include "Plugin/Internal/Game/GameGameplaySettings.hpp"

#include "Plugin/Internal/Game/GameIniSettings.hpp"

namespace Plugin::Internal::Game
{
	static constexpr auto DIFFICULTY_SETTING = "GamePlay:iDifficulty"sv;

	GameGameplaySettings::GameGameplaySettings(const REX::NotNull<std::shared_ptr<GameIniSettings>>& a_iniSettings)
		: _iniSettings(a_iniSettings)
	{
	}

	GameGameplaySettings::~GameGameplaySettings() noexcept = default;

	void GameGameplaySettings::OnGameDataReady()
	{
		_difficultySetting = _iniSettings->GetSetting(DIFFICULTY_SETTING);
		if (!_difficultySetting || !_difficultySetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
		}
	}

	RE::DifficultyLevel GameGameplaySettings::GetDifficulty() const
	{
		if (!_difficultySetting) [[unlikely]] {
			REX::Assert(false);
			return RE::DifficultyLevel::kNormal;
		}

		const auto difficulty = static_cast<RE::DifficultyLevel>(_difficultySetting->GetInt());
		return std::clamp(difficulty, RE::DifficultyLevel::kMin, RE::DifficultyLevel::kMax);
	}

	void GameGameplaySettings::SetDifficulty(RE::DifficultyLevel a_value)
	{
		if (!_difficultySetting || !_difficultySetting->IsInt()) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto difficulty = std::clamp(a_value, RE::DifficultyLevel::kMin, RE::DifficultyLevel::kMax);
		_difficultySetting->SetInt(static_cast<std::int32_t>(difficulty));
	}
}
