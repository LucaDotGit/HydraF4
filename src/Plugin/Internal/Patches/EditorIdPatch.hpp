#pragma once

namespace Plugin::Internal::Patches
{
	class EditorIdPatch final
	{
	public:
		EditorIdPatch() = default;
		~EditorIdPatch() noexcept = default;

		EditorIdPatch(const EditorIdPatch&) = delete;
		EditorIdPatch(EditorIdPatch&&) = delete;

		EditorIdPatch& operator=(const EditorIdPatch&) = delete;
		EditorIdPatch& operator=(EditorIdPatch&&) = delete;

		void OnXseLoad(REL::HookStore& a_hookStore);
		void OnGameDataReady();
	};
}
