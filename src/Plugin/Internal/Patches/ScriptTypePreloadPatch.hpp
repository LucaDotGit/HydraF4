#pragma once

namespace Plugin::Internal::Patches
{
	class ScriptTypePreloadPatch final
	{
	public:
		ScriptTypePreloadPatch() = default;
		~ScriptTypePreloadPatch() noexcept = default;

		ScriptTypePreloadPatch(const ScriptTypePreloadPatch&) = delete;
		ScriptTypePreloadPatch(ScriptTypePreloadPatch&&) = delete;

		ScriptTypePreloadPatch& operator=(const ScriptTypePreloadPatch&) = delete;
		ScriptTypePreloadPatch& operator=(ScriptTypePreloadPatch&&) = delete;

		void OnGameDataReady();
	};
}
