#include "Plugin/Internal/Events/Managers/HudColorUpdateManager.hpp"

#include "Plugin/Internal/Events/Sources/HudColorUpdateSource.hpp"
#include "Plugin/Internal/Game/GameUIColors.hpp"

namespace Plugin::Internal::Events
{
	HudColorUpdateManager::HudColorUpdateManager(const REX::NotNull<std::shared_ptr<Game::GameUIColors>>& a_gameUIColors)
		: _gameUIColors(a_gameUIColors)
	{
	}

	HudColorUpdateManager::~HudColorUpdateManager() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl HudColorUpdateManager::ProcessEvent([[maybe_unused]] const RE::ApplyColorUpdateEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::ApplyColorUpdateEvent>* a_eventSource)
	{
		const auto color =
			RE::PowerArmor::PlayerInPowerArmor() ?
				_gameUIColors->GetPowerArmorRgb() :
				_gameUIColors->GetHudRgb();

		const auto hudColorUpdateEvent = Events::HudColorUpdateEvent{
			.red = color.red,
			.green = color.green,
			.blue = color.blue
		};

		Events::HudColorUpdateSource::GetEventSource()->Notify(hudColorUpdateEvent);
		return RE::BSEventNotifyControl::kContinue;
	}

	void HudColorUpdateManager::OnGameDataReady()
	{
		RegisterEvents();
	}

	void HudColorUpdateManager::RegisterEvents()
	{
		auto* eventSource = RE::ApplyColorUpdateEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void HudColorUpdateManager::UnregisterEvents()
	{
		auto* eventSource = RE::ApplyColorUpdateEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}
}
