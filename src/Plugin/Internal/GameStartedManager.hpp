#pragma once

namespace Plugin::Internal
{
	class GameStartedManager final
	{
	public:
		GameStartedManager() = default;
		~GameStartedManager() noexcept = default;

		GameStartedManager(const GameStartedManager&) = delete;
		GameStartedManager(GameStartedManager&&) = delete;

		GameStartedManager& operator=(const GameStartedManager&) = delete;
		GameStartedManager& operator=(GameStartedManager&&) = delete;

		[[nodiscard]] bool GetIsNewGame() const noexcept;
		[[nodiscard]] bool GetIsFirstGameLoad() const noexcept;

		void OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest) noexcept;
		void OnPostLoadGame(bool a_isSucceeded) noexcept;

	private:
		std::atomic<bool> _isNewGame{ false };
		std::atomic<bool> _isFirstGameLoad{ true };
		std::atomic<bool> _hasLoadedOnce{ false };
	};
}
