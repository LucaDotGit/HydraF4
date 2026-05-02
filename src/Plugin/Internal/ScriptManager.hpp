#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Script
{
	class ScriptDumper;
	class ScriptVariantMap;
	class ScriptVariantSet;
}

namespace Plugin::Internal
{
	class ScriptManager final
	{
	public:
		ScriptManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~ScriptManager() noexcept;

		ScriptManager(const ScriptManager&) = delete;
		ScriptManager(ScriptManager&&) = delete;

		ScriptManager& operator=(const ScriptManager&) = delete;
		ScriptManager& operator=(ScriptManager&&) = delete;

		[[nodiscard]] auto GetScriptDumper() const noexcept { return _scriptDumper; }
		[[nodiscard]] auto GetMemoryMap() const noexcept { return _memoryMap; }
		[[nodiscard]] auto GetMemorySet() const noexcept { return _memorySet; }
		[[nodiscard]] auto GetTemporaryMap() const noexcept { return _temporaryMap; }
		[[nodiscard]] auto GetTemporarySet() const noexcept { return _temporarySet; }

		void OnGameDataReady();
		void OnNewOrPostLoadGame(bool a_isSucceeded);

	private:
		REX::NotNull<std::shared_ptr<Script::ScriptDumper>> _scriptDumper;
		REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>> _memoryMap;
		REX::NotNull<std::shared_ptr<Script::ScriptVariantSet>> _memorySet;
		REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>> _temporaryMap;
		REX::NotNull<std::shared_ptr<Script::ScriptVariantSet>> _temporarySet;
	};
}
