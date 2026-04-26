#include "Plugin/Internal/Runners/MiscStatRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Runners
{
	static constexpr auto MISC_STATS_KEY = "miscStats"sv;
	static constexpr auto STAT_ID_KEY = "statId"sv;
	static constexpr auto STAT_NAME_KEY = "statName"sv;
	static constexpr auto STAT_TYPE_KEY = "statType"sv;
	static constexpr auto SHOW_IF_ZERO_KEY = "showIfZero"sv;

	static constexpr auto DEFAULT_MISC_STAT_VALUE = 0;
	static constexpr auto DEFAULT_MISC_STAT_SHOW_IF_ZERO = true;

	auto MiscStatRunner::MiscStatInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<MiscStatInfo, std::string>
	{
		const auto statIdIt = a_json.find(STAT_ID_KEY);
		if (statIdIt == a_json.end() || !statIdIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				STAT_ID_KEY);
		}

		const auto statNameIt = a_json.find(STAT_NAME_KEY);
		if (statNameIt == a_json.end() || !statNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				STAT_NAME_KEY);
		}

		const auto statTypeIt = a_json.find(STAT_TYPE_KEY);
		if (statTypeIt == a_json.end() || !statTypeIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				STAT_TYPE_KEY);
		}

		const auto showIfZeroIt = a_json.find(SHOW_IF_ZERO_KEY);
		if (showIfZeroIt != a_json.end() && !showIfZeroIt->is_boolean()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find boolean value for key "{}")"sv,
				SHOW_IF_ZERO_KEY);
		}

		auto statInfo = MiscStatInfo();
		statInfo.statId = statIdIt->get<std::string>();
		statInfo.statName = statNameIt->get<std::string>();
		statInfo.statType = statTypeIt->get<std::string>();

		if (showIfZeroIt != a_json.end()) {
			statInfo.showIfZero = showIfZeroIt->get<bool>();
		}

		return statInfo;
	}

	auto MiscStatRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto miscStatsIt = a_json.find(MISC_STATS_KEY);
		if (miscStatsIt == a_json.end() || !miscStatsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				MISC_STATS_KEY);
		}

		auto rootInfo = RootInfo();

		auto& miscStats = rootInfo.miscStats;
		miscStats.reserve(miscStatsIt->size());

		for (auto miscStatIt = miscStatsIt->begin(); miscStatIt != miscStatsIt->end(); miscStatIt++) {
			if (!miscStatIt->is_object()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find object element at index {} in array for key "{}")"sv,
					std::distance(miscStatsIt->begin(), miscStatIt), MISC_STATS_KEY);
			}

			auto miscStat = MiscStatInfo::FromJson(*miscStatIt);
			if (!miscStat.has_value()) {
				return std::unexpected(std::move(miscStat).error());
			}

			miscStats.emplace_back(std::make_shared<MiscStatInfo>(std::move(miscStat).value()));
		}

		return rootInfo;
	}

	MiscStatRunner::MiscStatRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension))
	{
	}

	MiscStatRunner::~MiscStatRunner() noexcept
	{
		Dispose();
	}

	auto MiscStatRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			REX::LogDebug(R"(Loaded file "{}" with {} misc stat(s))"sv,
				a_filePath.generic_string(), rootInfo->miscStats.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto MiscStatRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		auto& miscStats = RE::MiscStatManager::GetMiscStats();
		for (const auto& [filePath, rootInfo] : _rootMap) {
			RunMiscStats(miscStats, filePath, rootInfo->miscStats);
		}

		return {};
	}

	void MiscStatRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto MiscStatRunner::RunMiscStat(
		RE::BSTArray<RE::MiscStatManager::MiscStat>& a_miscStatHolder,
		const REX::NotNull<std::shared_ptr<MiscStatInfo>>& a_miscStat)
		-> std::optional<std::string>
	{
		const auto containsMiscStat = [&a_miscStatHolder](const RE::BSFixedString& a_id) {
			return std::ranges::any_of(a_miscStatHolder, [&a_id](const RE::MiscStatManager::MiscStat& a_stat) {
				return a_stat.id == a_id;
			});
		};

		const auto statId = RE::BSFixedString(a_miscStat->statId);
		if (containsMiscStat(statId)) [[unlikely]] {
			return REX::Format(R"(stat ID "{}" was already defined somewhere else)"sv,
				statId);
		}

		const auto& miscStatType = a_miscStat->statType;

		const auto* statTypeIt = MISC_STAT_TYPE_MAP.find(miscStatType);
		if (statTypeIt == MISC_STAT_TYPE_MAP.end()) [[unlikely]] {
			return REX::Format(R"(invalid misc stat type "{}")"sv,
				miscStatType);
		}

		auto nativeMiscStat = RE::MiscStatManager::MiscStat{
			.id = statId,
			.displayName = RE::BSFixedString(a_miscStat->statName),
			.value = DEFAULT_MISC_STAT_VALUE,
			.type = statTypeIt->second,
			.showIfZero = a_miscStat->showIfZero.value_or(DEFAULT_MISC_STAT_SHOW_IF_ZERO)
		};

		a_miscStatHolder.push_back(std::move(nativeMiscStat));
		return std::nullopt;
	}

	void MiscStatRunner::RunMiscStats(
		RE::BSTArray<RE::MiscStatManager::MiscStat>& a_miscStatHolder,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<MiscStatInfo>>> a_miscStats)
	{
		for (const auto& miscStat : a_miscStats) {
			const auto errorMessage = RunMiscStat(a_miscStatHolder, miscStat);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Registered misc stat "{}" from file "{}")"sv,
				miscStat->statId, a_filePath.generic_string());
		}
	}
}
