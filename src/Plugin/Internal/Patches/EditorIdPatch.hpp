#pragma once

namespace Plugin::Internal::Patches::EditorIdPatch
{
	void OnXseLoad(REL::HookStore& a_hookStore);
	void OnGameDataReady();
}
