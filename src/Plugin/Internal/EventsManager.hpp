#pragma once

namespace Plugin::Internal::Events
{
	class CrosshairRefManager;
	class HudColorUpdateManager;
	class EventSystem;
}

namespace Plugin::Internal
{
	class GameManager;
	class GameStartedManager;

	class EventsManager final
	{
	public:
		EventsManager(
			const REX::NotNull<std::shared_ptr<GameStartedManager>>& a_gameStartedManager,
			const REX::NotNull<std::shared_ptr<GameManager>>& a_gameManager);

		~EventsManager() noexcept;

		EventsManager(const EventsManager&) = delete;
		EventsManager(EventsManager&&) = delete;

		EventsManager& operator=(const EventsManager&) = delete;
		EventsManager& operator=(EventsManager&&) = delete;

		[[nodiscard]] auto GetEventSystem() const noexcept { return _eventSystem; }
		[[nodiscard]] auto GetCrosshairRefManager() const noexcept { return _crosshairRefManager; }
		[[nodiscard]] auto GetHudColorUpdateManager() const noexcept { return _hudColorUpdateManager; }

		void OnGameDataReady();
		void OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest);
		void OnPostLoadGame(bool a_isSucceeded);
		void OnNewOrPostLoadGame(bool a_isSucceeded);
		void OnPostSaveGame(const RE::BSFixedString& a_saveName);
		void OnDeleteGame(const RE::BSFixedString& a_saveName);

	private:
		REX::NotNull<std::shared_ptr<GameStartedManager>> _gameStartedManager;
		REX::NotNull<std::shared_ptr<Events::CrosshairRefManager>> _crosshairRefManager;
		REX::NotNull<std::shared_ptr<Events::HudColorUpdateManager>> _hudColorUpdateManager;
		REX::NotNull<std::shared_ptr<Events::EventSystem>> _eventSystem;
	};
}
