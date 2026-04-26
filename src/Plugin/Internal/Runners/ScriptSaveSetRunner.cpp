#include "Plugin/Internal/Runners/ScriptSaveSetRunner.hpp"

#include "Plugin/Internal/Save/SaveScriptVariantSet.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto ROOT_NAME = "saveSets"sv;

	ScriptSaveSetRunner::ScriptSaveSetRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<Save::SaveScriptVariantSet>>& a_saveScriptVariantSet)
		: IScriptNamespaceRunner(std::move(a_directoryPath), std::move(a_fileExtension), a_saveScriptVariantSet)
	{
	}

	ScriptSaveSetRunner::~ScriptSaveSetRunner() noexcept = default;

	std::string_view ScriptSaveSetRunner::GetRootName() const noexcept
	{
		return ROOT_NAME;
	}
}
