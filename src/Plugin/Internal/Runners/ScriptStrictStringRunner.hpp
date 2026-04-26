#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	class ScriptStrictStringRunner final
		: public IFileRunner
	{
	public:
		class RootInfo final
			: public std::enable_shared_from_this<RootInfo>
		{
		public:
			RootInfo() = default;
			~RootInfo() noexcept = default;

			RootInfo(const RootInfo&) = default;
			RootInfo(RootInfo&&) noexcept = default;

			RootInfo& operator=(const RootInfo&) = default;
			RootInfo& operator=(RootInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<RootInfo, std::string>;

			std::vector<std::pair<std::string, std::string>> strictStrings;
		};

		ScriptStrictStringRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		~ScriptStrictStringRunner() noexcept override;

		ScriptStrictStringRunner(const ScriptStrictStringRunner&) = delete;
		ScriptStrictStringRunner(ScriptStrictStringRunner&&) = delete;

		ScriptStrictStringRunner& operator=(const ScriptStrictStringRunner&) = delete;
		ScriptStrictStringRunner& operator=(ScriptStrictStringRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		static auto RunScriptString(
			const std::pair<std::string, std::string>& a_strictStringPair)
			-> std::optional<std::string>;
		static void RunScriptStrings(
			const std::filesystem::path& a_filePath,
			const REX::NotNull<std::shared_ptr<RootInfo>>& a_rootInfo);

		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
