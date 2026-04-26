#include "Plugin/Internal/Runners/ConsoleCommandRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto COMMANDS_KEY = "commands"sv;
	static constexpr auto TEXT_KEY = "text"sv;

	auto ConsoleCommandRunner::CommandInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<CommandInfo, std::string>
	{
		const auto textIt = a_json.find(TEXT_KEY);
		if (textIt == a_json.end() || !textIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				TEXT_KEY);
		}

		auto commandInfo = CommandInfo();
		commandInfo.text = textIt->get<std::string>();

		return commandInfo;
	}

	auto ConsoleCommandRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto commandsIt = a_json.find(COMMANDS_KEY);
		if (commandsIt == a_json.end() || !commandsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				COMMANDS_KEY);
		}

		auto rootInfo = RootInfo();

		auto& commands = rootInfo.commands;
		commands.reserve(commandsIt->size());

		for (auto commandIt = commandsIt->begin(); commandIt != commandsIt->end(); commandIt++) {
			if (!commandIt->is_object()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find object element at index {} in array for key "{}")"sv,
					std::distance(commandsIt->begin(), commandIt), COMMANDS_KEY);
			}

			auto commandInfo = CommandInfo::FromJson(*commandIt);
			if (!commandInfo.has_value()) {
				return std::unexpected(std::move(commandInfo).error());
			}

			commands.emplace_back(std::make_shared<CommandInfo>(std::move(commandInfo).value()));
		}

		return rootInfo;
	}

	ConsoleCommandRunner::ConsoleCommandRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _settingsManager(a_settingsManager)
	{
	}

	ConsoleCommandRunner::~ConsoleCommandRunner() noexcept
	{
		Dispose();
	}

	auto ConsoleCommandRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto filePaths = IFileRunner::LoadDirectoryFiles();
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			REX::Fail(R"(Failed to iterate directory "{}": {})"sv,
				_directoryPath.generic_string(), filePaths.error());
		}

		return a_threadPool.SubmitTaskRange(std::move(filePaths).value(), [this](const std::filesystem::path& a_filePath) {
			auto jsonRoot = IFileRunner::json_t();

			try {
				auto fileStream = std::fstream(a_filePath, std::ios::in);
				jsonRoot = IFileRunner::json_t::parse(fileStream,
					REX::Json::GetParserCallback<IFileRunner::json_t>(),
					REX::Json::ALLOW_EXCEPTIONS,
					REX::Json::IGNORE_COMMENTS);
			}
			catch (const IFileRunner::json_t::exception& error) {
				REX::LogError(R"(Failed to parse file "{}": {})"sv,
					a_filePath.generic_string(), error.what());
				return;
			}

			if (jsonRoot.empty()) {
				return;
			}

			auto rootInfo = RootInfo::FromJson(jsonRoot);
			if (!rootInfo.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					a_filePath.generic_string(), rootInfo.error());
				return;
			}

			auto& commands = rootInfo->commands;

			REX::LogDebug(R"(Loaded file "{}" with {} command(s))"sv,
				a_filePath.generic_string(), commands.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ConsoleCommandRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto isSilent = !_settingsManager->GetPrintCommandRunnerCommandsSetting()->GetValue();
		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			RunCommands(filePath, rootInfo->commands, isSilent);
		}

		return {};
	}

	void ConsoleCommandRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ConsoleCommandRunner::RunCommand(
		const REX::NotNull<std::shared_ptr<CommandInfo>>& a_command,
		bool a_isSilent)
		-> std::optional<std::string>
	{
		constexpr auto TARGET_REF = static_cast<RE::TESObjectREFR*>(nullptr);

		const auto& commandText = a_command->text;
		if (!RE::Script::ExecuteMultiLineConsoleCommand(commandText, TARGET_REF, a_isSilent)) [[unlikely]] {
			return REX::Format(R"(failed to execute command: {})"sv,
				commandText);
		}

		return std::nullopt;
	}

	void ConsoleCommandRunner::RunCommands(
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<CommandInfo>>> a_commands,
		bool a_isSilent)
	{
		for (const auto& command : a_commands) {
			const auto errorMessage = RunCommand(command, a_isSilent);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Executed command from file "{}": {})"sv,
				a_filePath.generic_string(), command->text);
		}
	}
}
