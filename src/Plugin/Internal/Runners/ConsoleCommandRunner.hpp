#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Runners
{
	class ConsoleCommandRunner final
		: public IFileRunner
	{
	public:
		class CommandInfo final
			: public std::enable_shared_from_this<CommandInfo>
		{
		public:
			CommandInfo() = default;
			~CommandInfo() noexcept = default;

			CommandInfo(const CommandInfo&) = default;
			CommandInfo(CommandInfo&&) noexcept = default;

			CommandInfo& operator=(const CommandInfo&) = default;
			CommandInfo& operator=(CommandInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<CommandInfo, std::string>;

			std::string text;
		};

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

			std::vector<REX::NotNull<std::shared_ptr<CommandInfo>>> commands;
		};

		ConsoleCommandRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);

		~ConsoleCommandRunner() noexcept override;

		ConsoleCommandRunner(const ConsoleCommandRunner&) = delete;
		ConsoleCommandRunner(ConsoleCommandRunner&&) = delete;

		ConsoleCommandRunner& operator=(const ConsoleCommandRunner&) = delete;
		ConsoleCommandRunner& operator=(ConsoleCommandRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		static auto RunCommand(
			const REX::NotNull<std::shared_ptr<CommandInfo>>& a_command,
			bool a_isSilent)
			-> std::optional<std::string>;
		static void RunCommands(
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<CommandInfo>>> a_commands,
			bool a_isSilent);

		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
