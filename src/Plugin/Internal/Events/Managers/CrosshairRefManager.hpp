#pragma once

namespace Plugin::Internal::Events
{
	class CrosshairRefManager final
		: public RE::BSTEventSink<RE::ViewCasterUpdateEvent>
	{
	public:
		CrosshairRefManager();
		~CrosshairRefManager() noexcept override;

		CrosshairRefManager(const CrosshairRefManager&) = delete;
		CrosshairRefManager(CrosshairRefManager&&) = delete;

		CrosshairRefManager& operator=(const CrosshairRefManager&) = delete;
		CrosshairRefManager& operator=(CrosshairRefManager&&) = delete;

		[[nodiscard]] RE::ObjectRefHandle GetCurrentRef() const { return _currentRef; }

		RE::BSEventNotifyControl ProcessEvent(const RE::ViewCasterUpdateEvent& a_event,
			RE::BSTEventSource<RE::ViewCasterUpdateEvent>* a_eventSource) override;

		void OnGameDataReady();
		void OnPostLoadGame(bool a_isSucceeded);

		void RegisterEvents();
		void UnregisterEvents();
		void Clear();

	private:
		RE::ObjectRefHandle _previousRef;
		RE::ObjectRefHandle _currentRef;
	};
}
