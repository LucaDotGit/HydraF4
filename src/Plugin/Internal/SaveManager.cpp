#include "Plugin/Internal/SaveManager.hpp"

#include "Plugin/Internal/EventsManager.hpp"

#include "Plugin/Internal/Save/SaveEventSystem.hpp"
#include "Plugin/Internal/Save/SaveScriptFunctionMap.hpp"
#include "Plugin/Internal/Save/SaveScriptObjectMap.hpp"
#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp"
#include "Plugin/Internal/Save/SaveScriptVariantSet.hpp"

namespace Plugin::Internal
{
	static_assert(REX::HashCrc32("123456789"sv) == 0xCBF43926);

	static constexpr auto EVENT_SYSTEM_SAVE_TYPE = REX::HashCrc32("EventSystem"sv);
	static constexpr auto EVENT_SYSTEM_SAVE_VERSION = 1ui32;

	static constexpr auto SCRIPT_FUNCTION_MAP_SAVE_TYPE = REX::HashCrc32("ScriptFunctionMap"sv);
	static constexpr auto SCRIPT_FUNCTION_MAP_SAVE_VERSION = 1ui32;

	static constexpr auto SCRIPT_OBJECT_MAP_SAVE_TYPE = REX::HashCrc32("ScriptObjectMap"sv);
	static constexpr auto SCRIPT_OBJECT_MAP_SAVE_VERSION = 1ui32;

	static constexpr auto SCRIPT_VARIANT_MAP_SAVE_TYPE = REX::HashCrc32("ScriptVariantMap"sv);
	static constexpr auto SCRIPT_VARIANT_MAP_SAVE_VERSION = 1ui32;

	static constexpr auto SCRIPT_VARIANT_SET_SAVE_TYPE = REX::HashCrc32("ScriptVariantSet"sv);
	static constexpr auto SCRIPT_VARIANT_SET_SAVE_VERSION = 1ui32;

	SaveManager::SaveManager(const REX::NotNull<std::shared_ptr<EventsManager>>& a_eventsManager)
		: _eventSystem(std::make_shared<Save::SaveEventSystem>(EVENT_SYSTEM_SAVE_TYPE, EVENT_SYSTEM_SAVE_VERSION, a_eventsManager->GetEventSystem())),
		  _scriptFunctionMap(std::make_shared<Save::SaveScriptFunctionMap>(SCRIPT_FUNCTION_MAP_SAVE_TYPE, SCRIPT_FUNCTION_MAP_SAVE_VERSION, a_eventsManager->GetEventSystem())),
		  _scriptObjectMap(std::make_shared<Save::SaveScriptObjectMap>(SCRIPT_OBJECT_MAP_SAVE_TYPE, SCRIPT_OBJECT_MAP_SAVE_VERSION)),
		  _scriptVariantMap(std::make_shared<Save::SaveScriptVariantMap>(SCRIPT_VARIANT_MAP_SAVE_TYPE, SCRIPT_VARIANT_MAP_SAVE_VERSION)),
		  _scriptVariantSet(std::make_shared<Save::SaveScriptVariantSet>(SCRIPT_VARIANT_SET_SAVE_TYPE, SCRIPT_VARIANT_SET_SAVE_VERSION))
	{
		_savableInstances = {
			_eventSystem,
			_scriptFunctionMap,
			_scriptObjectMap,
			_scriptVariantMap,
			_scriptVariantSet,
		};
	}

	SaveManager::~SaveManager() noexcept = default;
}
