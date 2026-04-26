#include "Plugin/Internal/Runners/ScriptStrictStringRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Script/ScriptStrictStringPool.hpp"

namespace Plugin::Internal::Runners
{
	auto ScriptStrictStringRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		auto rootInfo = RootInfo();

		auto& strictStrings = rootInfo.strictStrings;
		strictStrings.reserve(a_json.size());

		for (const auto& [key, node] : a_json.items()) {
			if (!node.is_string()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find string value for key "{}")"sv,
					key);
			}

			strictStrings.emplace_back(key, node.get<std::string>());
		}

		return rootInfo;
	}

	ScriptStrictStringRunner::ScriptStrictStringRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension))
	{
	}

	ScriptStrictStringRunner::~ScriptStrictStringRunner() noexcept
	{
		Dispose();
	}

	auto ScriptStrictStringRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			REX::LogDebug(R"(Loaded file "{}" with {} strict string(s))"sv,
				a_filePath.generic_string(), rootInfo->strictStrings.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ScriptStrictStringRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			RunScriptStrings(filePath, rootInfo);
		}

		return {};
	}

	void ScriptStrictStringRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ScriptStrictStringRunner::RunScriptString(
		const std::pair<std::string, std::string>& a_strictStringPair)
		-> std::optional<std::string>
	{
		const auto& [key, value] = a_strictStringPair;

		if (!Script::ScriptStrictStringPool::IsKeyValid(static_cast<std::string_view>(key))) [[unlikely]] {
			return REX::Format(R"(invalid or missing key prefix ({}) for key "{}")"sv,
				Script::ScriptStrictStringPool::KEY_PREFIX<char>, key);
		}

		const auto nativeKey = RE::BSFixedString(key);
		const auto nativeValue = RE::BSFixedStringCS(value);

		if (!Script::GetScriptStrictStringPool()->AddValue(nativeKey, nativeValue)) [[unlikely]] {
			return REX::Format(R"(key "{}" already exists in the strict string pool)"sv,
				nativeKey);
		}

		return std::nullopt;
	}

	void ScriptStrictStringRunner::RunScriptStrings(
		const std::filesystem::path& a_filePath,
		const REX::NotNull<std::shared_ptr<RootInfo>>& a_rootInfo)
	{
		for (const auto& strictStringPair : a_rootInfo->strictStrings) {
			auto runStrictStringResult = RunScriptString(strictStringPair);
			if (runStrictStringResult.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), runStrictStringResult.value());
				continue;
			}

			REX::LogTrace(R"(Added strict string for key "{}" from file "{}")"sv,
				strictStringPair.first, a_filePath.generic_string());
		}
	}
}
