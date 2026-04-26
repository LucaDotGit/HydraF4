#include "Plugin/Internal/Runners/ScriptSaveMapRunner.hpp"

#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto ROOT_NAME = "saveMaps"sv;

	ScriptSaveMapRunner::ScriptSaveMapRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<Save::SaveScriptVariantMap>>& a_saveScriptVariantMap)
		: IScriptNamespaceRunner(std::move(a_directoryPath), std::move(a_fileExtension), a_saveScriptVariantMap)
	{
	}

	ScriptSaveMapRunner::~ScriptSaveMapRunner() noexcept = default;

	std::string_view ScriptSaveMapRunner::GetRootName() const noexcept
	{
		return ROOT_NAME;
	}
}
