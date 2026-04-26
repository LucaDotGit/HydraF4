#pragma once

namespace Plugin::Internal::Patches::ScriptFunctionTaskletPatch
{
	void OnXseLoad(REL::HookStore& a_hookStore);
	void OnGameDataReady();

	[[nodiscard]] bool IsDelayedFunction(const RE::BSFixedString& a_scriptName, const RE::BSFixedString& a_functionName) noexcept;
}
