#pragma once

namespace Plugin::Internal
{
	class EventsManager;
}

namespace Plugin::Internal::Save
{
	class ISaveObject;
	class SaveEventSystem;
	class SaveScriptFunctionMap;
	class SaveScriptObjectMap;
	class SaveScriptVariantMap;
	class SaveScriptVariantSet;
}

namespace Plugin::Internal
{
	class SaveManager final
	{
	public:
		SaveManager(const REX::NotNull<std::shared_ptr<EventsManager>>& a_eventsManager);
		~SaveManager() noexcept;

		SaveManager(const SaveManager&) = delete;
		SaveManager(SaveManager&&) = delete;

		SaveManager& operator=(const SaveManager&) = delete;
		SaveManager& operator=(SaveManager&&) = delete;

		[[nodiscard]] auto GetEventSystem() const noexcept { return _eventSystem; }
		[[nodiscard]] auto GetScriptFunctionMap() const noexcept { return _scriptFunctionMap; }
		[[nodiscard]] auto GetScriptObjectMap() const noexcept { return _scriptObjectMap; }
		[[nodiscard]] auto GetScriptVariantMap() const noexcept { return _scriptVariantMap; }
		[[nodiscard]] auto GetScriptVariantSet() const noexcept { return _scriptVariantSet; }

		[[nodiscard]] auto GetSavableInstances() const noexcept { return std::span(_savableInstances); }

	private:
		REX::NotNull<std::shared_ptr<Save::SaveEventSystem>> _eventSystem;
		REX::NotNull<std::shared_ptr<Save::SaveScriptFunctionMap>> _scriptFunctionMap;
		REX::NotNull<std::shared_ptr<Save::SaveScriptObjectMap>> _scriptObjectMap;
		REX::NotNull<std::shared_ptr<Save::SaveScriptVariantMap>> _scriptVariantMap;
		REX::NotNull<std::shared_ptr<Save::SaveScriptVariantSet>> _scriptVariantSet;
		std::vector<REX::NotNull<std::shared_ptr<Save::ISaveObject>>> _savableInstances;
	};
}
