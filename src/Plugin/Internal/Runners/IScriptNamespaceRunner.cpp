#include "Plugin/Internal/Runners/IScriptNamespaceRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Save/ISaveScriptVariantCollection.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto NAMESPACE_KEY = "namespace"sv;

	auto IScriptNamespaceRunner::NamespaceInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<NamespaceInfo, std::string>
	{
		auto rootInfo = NamespaceInfo();

		const auto nameIt = a_json.find(NAMESPACE_KEY);
		if (nameIt == a_json.end() || !nameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				NAMESPACE_KEY);
		}

		rootInfo.name = nameIt->get<std::string>();
		return rootInfo;
	}

	auto IScriptNamespaceRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json, std::string_view a_rootName)
		-> std::expected<RootInfo, std::string>
	{
		const auto namespacesIt = a_json.find(a_rootName);
		if (namespacesIt == a_json.end() || !namespacesIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				a_rootName);
		}

		auto rootInfo = RootInfo();

		for (auto namespaceIt = namespacesIt->begin(); namespaceIt != namespacesIt->end(); namespaceIt++) {
			if (!namespaceIt->is_object()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find object element at index {} in array for key "{}")"sv,
					std::distance(namespacesIt->begin(), namespaceIt), a_rootName);
			}

			auto namespaceInfo = NamespaceInfo::FromJson(*namespaceIt);
			if (!namespaceInfo.has_value()) {
				return std::unexpected(std::move(namespaceInfo).error());
			}

			rootInfo.names.push_back(std::move(namespaceInfo).value());
		}

		return rootInfo;
	}

	IScriptNamespaceRunner::IScriptNamespaceRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<Save::ISaveScriptVariantCollection>>& a_saveScriptVariantCollection)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _saveScriptVariantCollection(a_saveScriptVariantCollection)
	{
	}

	IScriptNamespaceRunner::~IScriptNamespaceRunner() noexcept = default;

	auto IScriptNamespaceRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			auto rootInfo = RootInfo::FromJson(jsonRoot, GetRootName());
			if (!rootInfo.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					a_filePath.generic_string(), rootInfo.error());
				return;
			}

			auto& names = rootInfo->names;

			REX::LogDebug(R"(Loaded file "{}" with {} namespace(s))"sv,
				a_filePath.generic_string(), names.size());

			const auto namespaceLock = std::scoped_lock(_mapMutex);

			for (const auto& namespaceInfo : names) {
				_validNamespaceMap.emplace(a_filePath, namespaceInfo.name);
			}
		});
	}

	auto IScriptNamespaceRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto namespaceLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, namespaceName] : _validNamespaceMap) {
			_saveScriptVariantCollection->AddValidNamespace(namespaceName);

			REX::LogTrace(R"(Added namespace "{}" from file "{}")"sv,
				namespaceName, filePath.generic_string());
		}

		return {};
	}

	void IScriptNamespaceRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto namespaceLock = std::scoped_lock(_mapMutex);

		_validNamespaceMap.clear();
	}
}
