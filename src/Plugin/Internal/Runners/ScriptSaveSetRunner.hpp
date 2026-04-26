#pragma once

#include "Plugin/Internal/Runners/IScriptNamespaceRunner.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptVariantSet;
}

namespace Plugin::Internal::Runners
{
	class ScriptSaveSetRunner final
		: public IScriptNamespaceRunner
	{
	public:
		ScriptSaveSetRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<Save::SaveScriptVariantSet>>& a_saveScriptVariantSet);

		~ScriptSaveSetRunner() noexcept override;

		ScriptSaveSetRunner(const ScriptSaveSetRunner&) = delete;
		ScriptSaveSetRunner(ScriptSaveSetRunner&&) = delete;

		ScriptSaveSetRunner& operator=(const ScriptSaveSetRunner&) = delete;
		ScriptSaveSetRunner& operator=(ScriptSaveSetRunner&&) = delete;

		[[nodiscard]] std::string_view GetRootName() const noexcept override;
	};
}
