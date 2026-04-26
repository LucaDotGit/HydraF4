#include "Plugin/Internal/EventsManager.hpp"

#include "Plugin/Internal/Events/EventSystem.hpp"
#include "Plugin/Internal/Events/Managers/CrosshairRefManager.hpp"
#include "Plugin/Internal/Events/Managers/HudColorUpdateManager.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/GameStartedManager.hpp"

#include "Plugin/Internal/Events/Sources/ActorValueChangeSource.hpp"
#include "Plugin/Internal/Events/Sources/AnimationGraphEventSource.hpp"
#include "Plugin/Internal/Events/Sources/ButtonUpDownSource.hpp"
#include "Plugin/Internal/Events/Sources/DeleteGameSource.hpp"
#include "Plugin/Internal/Events/Sources/HudColorUpdateSource.hpp"
#include "Plugin/Internal/Events/Sources/LifeStateChangeSource.hpp"
#include "Plugin/Internal/Events/Sources/NewGameSource.hpp"
#include "Plugin/Internal/Events/Sources/PostLoadGameSource.hpp"
#include "Plugin/Internal/Events/Sources/PostSaveGameSource.hpp"
#include "Plugin/Internal/Events/Sources/StartGameSource.hpp"
#include "Plugin/Internal/Events/Sources/UserEventSource.hpp"

namespace Plugin::Internal
{
	EventsManager::EventsManager(
		const REX::NotNull<std::shared_ptr<GameStartedManager>>& a_gameStartedManager,
		const REX::NotNull<std::shared_ptr<GameManager>>& a_gameManager)
		: _gameStartedManager(a_gameStartedManager),
		  _crosshairRefManager(std::make_shared<Events::CrosshairRefManager>()),
		  _hudColorUpdateManager(std::make_shared<Events::HudColorUpdateManager>(a_gameManager->GetUIColors())),
		  _eventSystem(std::make_shared<Events::EventSystem>())
	{
	}

	EventsManager::~EventsManager() noexcept = default;

	void EventsManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		std::ignore = Events::ActorValueChangeSource::GetEventSource();
		std::ignore = Events::AnimationGraphEventSource::GetEventSource();
		std::ignore = Events::ButtonUpDownSource::GetEventSource();
		std::ignore = Events::DeleteGameSource::GetEventSource();
		std::ignore = Events::HudColorUpdateSource::GetEventSource();
		std::ignore = Events::LifeStateChangeSource::GetEventSource();
		std::ignore = Events::NewGameSource::GetEventSource();
		std::ignore = Events::PostLoadGameSource::GetEventSource();
		std::ignore = Events::PostSaveGameSource::GetEventSource();
		std::ignore = Events::UserEventSource::GetEventSource();

		_crosshairRefManager->OnGameDataReady();
		_hudColorUpdateManager->OnGameDataReady();

		_eventSystem->OnGameDataReady();
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void EventsManager::OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto newGameEvent = Events::NewGameEvent{ .charGenQuest = a_charGenQuest.get() };
		Events::NewGameSource::GetEventSource()->Notify(newGameEvent);
	}

	void EventsManager::OnPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_crosshairRefManager->OnPostLoadGame(a_isSucceeded);
	}

	void EventsManager::OnNewOrPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		if (_gameStartedManager->GetIsFirstGameLoad()) {
			const auto startGameEvent = Events::StartGameEvent{ .isSucceeded = a_isSucceeded };
			Events::StartGameSource::GetEventSource()->Notify(startGameEvent);
		}

		const auto postLoadGameEvent = Events::PostLoadGameEvent{ .isSucceeded = a_isSucceeded };
		Events::PostLoadGameSource::GetEventSource()->Notify(postLoadGameEvent);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void EventsManager::OnPostSaveGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto postSaveGameEvent = Events::PostSaveGameEvent{ .saveName = a_saveName };
		Events::PostSaveGameSource::GetEventSource()->Notify(postSaveGameEvent);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void EventsManager::OnDeleteGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto deleteGameEvent = Events::DeleteGameEvent{ .saveName = a_saveName };
		Events::DeleteGameSource::GetEventSource()->Notify(deleteGameEvent);
	}
}
