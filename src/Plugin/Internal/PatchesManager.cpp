#include "Plugin/Internal/PatchesManager.hpp"

#include "Plugin/Internal/Patches/EditorIdPatch.hpp"

#include "Plugin/Internal/Patches/ScriptTypePreloadPatch.hpp"

#include "Plugin/Internal/Patches/ScriptFunctionTaskletPatch.hpp"

namespace Plugin::Internal
{
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void PatchesManager::OnXseLoad()
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, []() noexcept {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			const auto& hookStore = REL::GetHookStore();

			Patches::EditorIdPatch::OnXseLoad(*(hookStore.get()));
			Patches::ScriptFunctionTaskletPatch::OnXseLoad(*(hookStore.get()));
		});
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void PatchesManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Patches::EditorIdPatch::OnGameDataReady();
		Patches::ScriptTypePreloadPatch::OnGameDataReady();
		Patches::ScriptFunctionTaskletPatch::OnGameDataReady();
	}
}
