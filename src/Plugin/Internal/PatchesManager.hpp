#pragma once

namespace Plugin::Internal::Patches
{
	class EditorIdPatch;
	class ScriptTypePreloadPatch;
	class ScriptFunctionTaskletPatch;
}

namespace Plugin::Internal
{
	class SettingsManager;

	class PatchesManager final
	{
	public:
		PatchesManager(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~PatchesManager() noexcept;

		PatchesManager(const PatchesManager&) = delete;
		PatchesManager(PatchesManager&&) = delete;

		PatchesManager& operator=(const PatchesManager&) = delete;
		PatchesManager& operator=(PatchesManager&&) = delete;

		[[nodiscard]] auto GetEditorIdPatch() const noexcept { return _editorIdPatch; }
		[[nodiscard]] auto GetScriptTypePreloadPatch() const noexcept { return _scriptTypePreloadPatch; }
		[[nodiscard]] auto GetScriptFunctionTaskletPatch() const noexcept { return _scriptFunctionTaskletPatch; }

		void OnXseLoad();
		void OnGameDataReady();

	private:
		REX::NotNull<std::shared_ptr<Patches::EditorIdPatch>> _editorIdPatch;
		REX::NotNull<std::shared_ptr<Patches::ScriptTypePreloadPatch>> _scriptTypePreloadPatch;
		REX::NotNull<std::shared_ptr<Patches::ScriptFunctionTaskletPatch>> _scriptFunctionTaskletPatch;
	};
}
