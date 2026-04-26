#include "Plugin/Internal/Events/Managers/CrosshairRefManager.hpp"

#include "Plugin/Internal/Events/Sources/CrosshairRefChangeSource.hpp"

namespace Plugin::Internal::Events
{
	CrosshairRefManager::CrosshairRefManager() = default;

	CrosshairRefManager::~CrosshairRefManager() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});
	}

	RE::BSEventNotifyControl CrosshairRefManager::ProcessEvent(const RE::ViewCasterUpdateEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::ViewCasterUpdateEvent>* a_eventSource)
	{
		const auto& value = a_event.optionalValue;

		const auto previousRefHandle = _currentRef;
		const auto currentRefHandle = value.has_value() ? value->data.activatePickRef : RE::ObjectRefHandle();

		const auto previousRef = _currentRef.get();
		const auto currentRef = currentRefHandle.get();

		_previousRef = previousRefHandle;
		_currentRef = currentRefHandle;

		if (previousRef == currentRef) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (currentRef && previousRef) {
			auto crosshairRefChangeEvent = Events::CrosshairRefChangeEvent{
				.targetRef = previousRef,
				.isTargeted = false,
			};

			Events::CrosshairRefChangeSource::GetEventSource()->Notify(crosshairRefChangeEvent);
		}

		const auto selectedRef = currentRef ? currentRef : previousRef;
		const auto isTargeted = selectedRef == currentRef;

		auto crosshairRefChangeEvent = Events::CrosshairRefChangeEvent{
			.targetRef = selectedRef,
			.isTargeted = isTargeted,
		};

		Events::CrosshairRefChangeSource::GetEventSource()->Notify(crosshairRefChangeEvent);
		return RE::BSEventNotifyControl::kContinue;
	}

	void CrosshairRefManager::OnGameDataReady()
	{
		RegisterEvents();
	}

	void CrosshairRefManager::OnPostLoadGame(bool a_isSucceeded)
	{
		if (!a_isSucceeded) {
			return;
		}

		Clear();
	}

	void CrosshairRefManager::RegisterEvents()
	{
		auto* eventSource = RE::ViewCasterUpdateEvent::GetEventSource();
		if (eventSource) {
			eventSource->RegisterSink(this);
		}
	}

	void CrosshairRefManager::UnregisterEvents()
	{
		auto* eventSource = RE::ViewCasterUpdateEvent::GetEventSource();
		if (eventSource) {
			eventSource->UnregisterSink(this);
		}
	}

	void CrosshairRefManager::Clear()
	{
		_previousRef.reset();
		_currentRef.reset();
	}
}
