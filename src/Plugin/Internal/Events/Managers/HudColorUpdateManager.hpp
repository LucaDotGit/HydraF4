#pragma once

namespace Plugin::Internal::Game
{
	class GameUIColors;
}

namespace Plugin::Internal::Events
{
	class HudColorUpdateManager final
		: public RE::BSTEventSink<RE::ApplyColorUpdateEvent>
	{
	public:
		HudColorUpdateManager(const REX::NotNull<std::shared_ptr<Game::GameUIColors>>& a_gameUIColors);
		~HudColorUpdateManager() noexcept override;

		HudColorUpdateManager(const HudColorUpdateManager&) = delete;
		HudColorUpdateManager(HudColorUpdateManager&&) = delete;

		HudColorUpdateManager& operator=(const HudColorUpdateManager&) = delete;
		HudColorUpdateManager& operator=(HudColorUpdateManager&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::ApplyColorUpdateEvent& a_event,
			RE::BSTEventSource<RE::ApplyColorUpdateEvent>* a_eventSource) override;

		void OnGameDataReady();

		void RegisterEvents();
		void UnregisterEvents();

	private:
		REX::NotNull<std::shared_ptr<Game::GameUIColors>> _gameUIColors;
	};
}
