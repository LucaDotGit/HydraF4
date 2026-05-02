#include "Plugin/Internal/ScriptManager.hpp"

#include "Plugin/Internal/Script/ScriptDumper.hpp"
#include "Plugin/Internal/Script/ScriptVariantMap.hpp"
#include "Plugin/Internal/Script/ScriptVariantSet.hpp"

namespace Plugin::Internal
{
	ScriptManager::ScriptManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _scriptDumper(std::make_shared<Script::ScriptDumper>(a_settingsManager)),
		  _memoryMap(std::make_shared<Script::ScriptVariantMap>()),
		  _memorySet(std::make_shared<Script::ScriptVariantSet>()),
		  _temporaryMap(std::make_shared<Script::ScriptVariantMap>()),
		  _temporarySet(std::make_shared<Script::ScriptVariantSet>())
	{
	}

	ScriptManager::~ScriptManager() noexcept = default;

	void ScriptManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_scriptDumper->OnGameDataReady();
	}

	void ScriptManager::OnNewOrPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		if (!a_isSucceeded) {
			return;
		}

		_temporaryMap->Clear();
		_temporarySet->Clear();
	}
}
