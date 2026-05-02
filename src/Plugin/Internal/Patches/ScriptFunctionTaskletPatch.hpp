#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Patches
{
	class ScriptFunctionTaskletPatch final
	{
	public:
		ScriptFunctionTaskletPatch(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~ScriptFunctionTaskletPatch() noexcept;

		ScriptFunctionTaskletPatch(const ScriptFunctionTaskletPatch&) = delete;
		ScriptFunctionTaskletPatch(ScriptFunctionTaskletPatch&&) = delete;

		ScriptFunctionTaskletPatch& operator=(const ScriptFunctionTaskletPatch&) = delete;
		ScriptFunctionTaskletPatch& operator=(ScriptFunctionTaskletPatch&&) = delete;

		void OnXseLoad(REL::HookStore& a_hookStore);
		void OnGameDataReady();

		[[nodiscard]] bool IsDelayedFunction(const RE::BSFixedString& a_scriptName, const RE::BSFixedString& a_functionName) const noexcept;

	private:
		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
	};
}
