#pragma once

#include "Plugin/Internal/Runners/IScriptNamespaceRunner.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptVariantMap;
}

namespace Plugin::Internal::Runners
{
	class ScriptSaveMapRunner final
		: public IScriptNamespaceRunner
	{
	public:
		ScriptSaveMapRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<Save::SaveScriptVariantMap>>& a_saveScriptVariantMap);

		~ScriptSaveMapRunner() noexcept override;

		ScriptSaveMapRunner(const ScriptSaveMapRunner&) = delete;
		ScriptSaveMapRunner(ScriptSaveMapRunner&&) = delete;

		ScriptSaveMapRunner& operator=(const ScriptSaveMapRunner&) = delete;
		ScriptSaveMapRunner& operator=(ScriptSaveMapRunner&&) = delete;

		[[nodiscard]] std::string_view GetRootName() const noexcept override;
	};
}
