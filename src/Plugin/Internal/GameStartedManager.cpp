#include "Plugin/Internal/GameStartedManager.hpp"

namespace Plugin::Internal
{
	bool GameStartedManager::GetIsNewGame() const noexcept
	{
		return _isNewGame.load(std::memory_order_acquire);
	}

	bool GameStartedManager::GetIsFirstGameLoad() const noexcept
	{
		return _isFirstGameLoad.load(std::memory_order_acquire);
	}

	void GameStartedManager::OnNewGame([[maybe_unused]] REX::NotNull<RE::TESQuest*> a_charGenQuest) noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_isNewGame.store(true, std::memory_order_release);
	}

	void GameStartedManager::OnPostLoadGame(bool a_isSucceeded) noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		if (!a_isSucceeded) {
			return;
		}

		if (_hasLoadedOnce.load(std::memory_order_acquire)) {
			_isNewGame.store(false, std::memory_order_release);
			_isFirstGameLoad.store(false, std::memory_order_release);
			return;
		}

		_isNewGame.store(false, std::memory_order_release);
		_isFirstGameLoad.store(true, std::memory_order_release);

		_hasLoadedOnce.store(true, std::memory_order_release);
	}
}
