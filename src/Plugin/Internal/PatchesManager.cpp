#include "Plugin/Internal/PatchesManager.hpp"

#include "Plugin/Internal/SettingsManager.hpp"

#include "Plugin/Internal/Patches/EditorIdPatch.hpp"

#include "Plugin/Internal/Patches/ScriptTypePreloadPatch.hpp"

#include "Plugin/Internal/Patches/ScriptFunctionTaskletPatch.hpp"

namespace Plugin::Internal
{
	PatchesManager::PatchesManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _editorIdPatch(std::make_shared<Patches::EditorIdPatch>()),
		  _scriptTypePreloadPatch(std::make_shared<Patches::ScriptTypePreloadPatch>()),
		  _scriptFunctionTaskletPatch(std::make_shared<Patches::ScriptFunctionTaskletPatch>(a_settingsManager))
	{
	}

	PatchesManager::~PatchesManager() noexcept = default;

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void PatchesManager::OnXseLoad()
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [this]() noexcept {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			const auto& hookStore = REL::GetHookStore();

			_editorIdPatch->OnXseLoad(*(hookStore.get()));
			_scriptFunctionTaskletPatch->OnXseLoad(*(hookStore.get()));
		});
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void PatchesManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		_editorIdPatch->OnGameDataReady();
		_scriptTypePreloadPatch->OnGameDataReady();
		_scriptFunctionTaskletPatch->OnGameDataReady();
	}
}
