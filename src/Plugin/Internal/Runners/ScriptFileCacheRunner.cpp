#include "Plugin/Internal/Runners/ScriptFileCacheRunner.hpp"

#include "Plugin/Internal/Core/IniUtility.hpp"
#include "Plugin/Internal/Core/JsonUtility.hpp"
#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Core/TomlUtility.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Script/ScriptVariantMap.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto CACHED_FILES_KEY = "cachedFiles"sv;
	static constexpr auto FILE_TYPE_KEY = "fileType"sv;
	static constexpr auto DIRECTORY_PATH_KEY = "directoryPath"sv;
	static constexpr auto FILE_NAME_PATTERN_KEY = "fileNamePattern"sv;
	static constexpr auto MAP_NAMESPACE_KEY = "mapNamespace"sv;
	static constexpr auto SEARCH_RECURSIVELY_KEY = "searchRecursively"sv;

	static constexpr auto TEXT_FILE_TYPE = "Text"sv;
	static constexpr auto INI_FILE_TYPE = "Ini"sv;
	static constexpr auto TOML_FILE_TYPE = "Toml"sv;
	static constexpr auto JSON_FILE_TYPE = "Json"sv;

	static constexpr auto DEFAULT_SEARCH_RECURSIVELY = false;

	auto ScriptFileCacheRunner::CachedFileInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<CachedFileInfo, std::string>
	{
		const auto fileTypeIt = a_json.find(FILE_TYPE_KEY);
		if (fileTypeIt == a_json.end() || !fileTypeIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				FILE_TYPE_KEY);
		}

		const auto directoryPathIt = a_json.find(DIRECTORY_PATH_KEY);
		if (directoryPathIt == a_json.end() || !directoryPathIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				DIRECTORY_PATH_KEY);
		}

		const auto fileNamePatternIt = a_json.find(FILE_NAME_PATTERN_KEY);
		if (fileNamePatternIt == a_json.end() || !fileNamePatternIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				FILE_NAME_PATTERN_KEY);
		}

		const auto mapNamespaceIt = a_json.find(MAP_NAMESPACE_KEY);
		if (mapNamespaceIt == a_json.end() || !mapNamespaceIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				MAP_NAMESPACE_KEY);
		}

		const auto searchRecursivelyIt = a_json.find(SEARCH_RECURSIVELY_KEY);
		if (searchRecursivelyIt != a_json.end() && !searchRecursivelyIt->is_boolean()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find boolean value for key "{}")"sv,
				SEARCH_RECURSIVELY_KEY);
		}

		auto cachedFileInfo = CachedFileInfo();
		cachedFileInfo.fileType = fileTypeIt->get<std::string>();
		cachedFileInfo.directoryPath = directoryPathIt->get<std::filesystem::path>();
		cachedFileInfo.fileNamePattern = fileNamePatternIt->get<std::filesystem::path>();
		cachedFileInfo.mapNamespace = mapNamespaceIt->get<std::string>();

		if (searchRecursivelyIt != a_json.end()) {
			cachedFileInfo.searchRecursively = searchRecursivelyIt->get<bool>();
		}

		return cachedFileInfo;
	}

	auto ScriptFileCacheRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto cachedFilesIt = a_json.find(CACHED_FILES_KEY);
		if (cachedFilesIt == a_json.end() || !cachedFilesIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				CACHED_FILES_KEY);
		}

		auto rootInfo = RootInfo();

		auto cachedFiles = std::vector<REX::NotNull<std::shared_ptr<CachedFileInfo>>>();
		cachedFiles.reserve(cachedFilesIt->size());

		for (auto fileIt = cachedFilesIt->begin(); fileIt != cachedFilesIt->end(); fileIt++) {
			if (!fileIt->is_object()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find object element at index {} in array for key "{}")"sv,
					std::distance(cachedFilesIt->begin(), fileIt), CACHED_FILES_KEY);
			}

			auto fileInfo = CachedFileInfo::FromJson(*fileIt);
			if (!fileInfo.has_value()) {
				return std::unexpected(std::move(fileInfo).error());
			}

			cachedFiles.emplace_back(std::make_shared<CachedFileInfo>(std::move(fileInfo).value()));
		}

		rootInfo.cachedFiles = std::move(cachedFiles);

		return rootInfo;
	}

	ScriptFileCacheRunner::ScriptFileCacheRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<IO::IOSandbox>>& a_ioSandbox,
		const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _ioSandbox(a_ioSandbox),
		  _scriptVariantMapCacher(a_scriptVariantMap)
	{
		_fileCachers = {
			{ TEXT_FILE_TYPE, std::make_shared<TextFileCacher>() },
			{ INI_FILE_TYPE, std::make_shared<IniFileCacher>() },
			{ TOML_FILE_TYPE, std::make_shared<TomlFileCacher>() },
			{ JSON_FILE_TYPE, std::make_shared<JsonFileCacher>() }
		};
	}

	ScriptFileCacheRunner::~ScriptFileCacheRunner() noexcept
	{
		Dispose();
	}

	auto ScriptFileCacheRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			REX::LogDebug(R"(Loaded file "{}" with {} file(s)))"sv,
				a_filePath.generic_string(), rootInfo->cachedFiles.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ScriptFileCacheRunner::Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		return a_threadPool.SubmitTaskRange(_rootMap, [this](const std::pair<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>>& a_rootPair) {
			const auto& [filePath, rootInfo] = a_rootPair;
			const auto& cachedFiles = rootInfo->cachedFiles;

			for (const auto& cachedFileInfo : cachedFiles) {
				const auto errorMessages = RunDirectoryCaching(cachedFileInfo);
				if (errorMessages.empty()) [[likely]] {
					REX::LogTrace(R"(Cached files from directory "{}" with pattern "{}" for map namespace "{}" and file "{}")"sv,
						cachedFileInfo->directoryPath.generic_string(), cachedFileInfo->fileNamePattern, cachedFileInfo->mapNamespace, filePath.generic_string());
					continue;
				}

				for (const auto& errorMessage : errorMessages) {
					REX::LogError(R"(Error in file "{}"; {})"sv,
						filePath.generic_string(), errorMessage);
				}
			}
		});
	}

	void ScriptFileCacheRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ScriptFileCacheRunner::TextFileCacher::CacheFile(
		const std::filesystem::path& a_definitionFilePath,
		const RE::BSFixedString& a_mapNamespace,
		const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
		-> std::optional<std::string>
	{
		auto fileContent = std::string();

		try {
			auto fileStream = std::fstream();
			fileStream.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
			fileStream.open(a_definitionFilePath, std::ios::in);

			fileContent = { std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };
		}
		catch (const std::ios::failure& error) {
			return error.what();
		}

		auto scriptKey = RE::BSScript::Variable(RE::BSFixedString(a_definitionFilePath.lexically_normal().generic_string()));
		auto scriptValue = RE::BSScript::Variable(RE::BSFixedString(fileContent));

		a_scriptVariantMap->SetValue(a_mapNamespace, std::addressof(scriptKey), std::addressof(scriptValue));
		return std::nullopt;
	}

	auto ScriptFileCacheRunner::IniFileCacher::CacheFile(
		const std::filesystem::path& a_definitionFilePath,
		const RE::BSFixedString& a_mapNamespace,
		const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
		-> std::optional<std::string>
	{
		using ci_ini_t = Core::IniUtility::ini_t;

		auto iniFile = ci_ini_t();
		Core::IniUtility::SetupFile(iniFile);

		if (iniFile.LoadFile(a_definitionFilePath.native().data()) < SI_OK) [[unlikely]] {
			return "failed to load INI file"s;
		}

		auto sections = ci_ini_t::TNamesDepend();
		iniFile.GetAllSections(sections);

		auto pairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();

		for (const auto& section : sections) {
			auto keys = ci_ini_t::TNamesDepend();
			iniFile.GetAllKeys(section.pItem, keys);

			pairs.reserve(pairs.size() + keys.size());

			for (const auto& key : keys) {
				const auto iniValue = REX::Ini::GetDetectedValue(iniFile, section.pItem, key.pItem);
				if (!iniValue.has_value()) {
					continue;
				}

				auto scriptValue = Core::IniUtility::ToDetectedValue(iniValue.value());
				if (!scriptValue.has_value()) {
					continue;
				}

				auto iniSetting = REX::Ini::CombineSetting(
					std::string_view(section.pItem),
					std::string_view(key.pItem));

				auto scriptKey = RE::BSScript::Variable(RE::BSFixedString(std::move(iniSetting)));
				pairs.emplace_back(std::move(scriptKey), std::move(scriptValue).value());
			}
		}

		a_scriptVariantMap->SetRange(a_mapNamespace, pairs);
		return std::nullopt;
	}

	auto ScriptFileCacheRunner::TomlFileCacher::CacheFile(
		const std::filesystem::path& a_definitionFilePath,
		const RE::BSFixedString& a_mapNamespace,
		const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
		-> std::optional<std::string>
	{
		using ci_toml_t = Core::TomlUtility::toml_t;

		auto tomlRoot = ci_toml_t();

		try {
			auto result = toml::try_parse(a_definitionFilePath, Core::TomlUtility::SPECIFICATION_VERSION);
			if (!result.is_ok()) [[unlikely]] {
				return REX::Format("failed to parse TOML file: {}"sv,
					toml::format_error(result.as_err().back()));
			}

			tomlRoot = std::move(result).unwrap();
		}
		catch (const toml::exception& error) {
			return error.what();
		}

		auto flattenedRoot = REX::Toml::Flatten(tomlRoot);
		if (!flattenedRoot.has_value() || !flattenedRoot->is_table()) [[unlikely]] {
			return "failed to flatten TOML file"s;
		}

		auto pairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();
		pairs.reserve(flattenedRoot->as_table().size());

		for (const auto& [key, value] : flattenedRoot->as_table()) {
			auto scriptValue = Core::TomlUtility::ToDetectedValue(value);
			if (!scriptValue.has_value()) {
				continue;
			}

			auto scriptKey = RE::BSScript::Variable(RE::BSFixedString(key));
			pairs.emplace_back(std::move(scriptKey), std::move(scriptValue).value());
		}

		a_scriptVariantMap->SetRange(a_mapNamespace, pairs);
		return std::nullopt;
	}

	auto ScriptFileCacheRunner::JsonFileCacher::CacheFile(
		const std::filesystem::path& a_definitionFilePath,
		const RE::BSFixedString& a_mapNamespace,
		const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
		-> std::optional<std::string>
	{
		using ci_json_t = Core::JsonUtility::json_t;

		auto jsonRoot = ci_json_t();

		try {
			auto fileStream = std::fstream(a_definitionFilePath, std::ios::in);
			jsonRoot = ci_json_t::parse(fileStream,
				Core::JsonUtility::GetParserCallback(),
				Core::JsonUtility::ALLOW_EXCEPTIONS,
				Core::JsonUtility::IGNORE_COMMENTS);
		}
		catch (const ci_json_t::exception& error) {
			return error.what();
		}

		auto flattenedRoot = jsonRoot.flatten();
		if (!flattenedRoot.is_object()) [[unlikely]] {
			return "failed to flatten JSON file"s;
		}

		auto pairs = std::vector<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>>();
		pairs.reserve(flattenedRoot.size());

		for (const auto& [key, value] : flattenedRoot.items()) {
			auto scriptValue = Core::JsonUtility::ToDetectedValue(value);
			if (!scriptValue.has_value()) {
				continue;
			}

			auto scriptKey = RE::BSScript::Variable(RE::BSFixedString(key));
			pairs.emplace_back(std::move(scriptKey), std::move(scriptValue).value());
		}

		a_scriptVariantMap->SetRange(a_mapNamespace, pairs);
		return std::nullopt;
	}

	auto ScriptFileCacheRunner::RunDirectoryCaching(const REX::NotNull<std::shared_ptr<CachedFileInfo>>& a_cachedFileInfo) -> std::vector<std::string>
	{
		const auto& directoryPath = a_cachedFileInfo->directoryPath;
		if (!_ioSandbox->IsInSandbox(directoryPath)) [[unlikely]] {
			return {
				REX::Format(R"(directory "{}" is outside the sandbox)"sv,
					directoryPath.generic_string())
			};
		}

		const auto& fileNamePattern = a_cachedFileInfo->fileNamePattern;
		const auto searchRecursively = a_cachedFileInfo->searchRecursively.value_or(DEFAULT_SEARCH_RECURSIVELY);

		const auto filePaths = REX::GetFilesInDirectory(directoryPath, fileNamePattern, searchRecursively);
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			return {
				REX::Format(R"(failed to iterate directory "{}" with pattern "{}"; {})"sv,
					directoryPath.generic_string(), fileNamePattern.generic_string(), filePaths.error().message())
			};
		}

		auto errorMessages = std::vector<std::string>();
		for (const auto& filePath : filePaths.value()) {
			auto errorMessage = RunFileCaching(a_cachedFileInfo, filePath);
			if (errorMessage.has_value()) [[unlikely]] {
				errorMessages.push_back(std::move(errorMessage).value());
			}
		}

		return errorMessages;
	}

	auto ScriptFileCacheRunner::RunFileCaching(
		const REX::NotNull<std::shared_ptr<CachedFileInfo>>& a_cachedFileInfo,
		const std::filesystem::path& a_definitionFilePath)
		-> std::optional<std::string>
	{
		const auto fileCacherIt = _fileCachers.find(a_cachedFileInfo->fileType);
		if (fileCacherIt == _fileCachers.end()) [[unlikely]] {
			return REX::Format(R"(failed to cache file "{}" with invalid file type "{}")"sv,
				a_definitionFilePath.generic_string(), a_cachedFileInfo->fileType);
		}

		const auto& fileCacher = fileCacherIt->second;

		const auto mapNamespace = RE::BSFixedString(a_cachedFileInfo->mapNamespace);
		_scriptVariantMapCacher->AddValidNamespace(mapNamespace);

		const auto cacheErrorMessage = fileCacher->CacheFile(a_definitionFilePath, mapNamespace, _scriptVariantMapCacher);
		if (cacheErrorMessage.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to cache file "{}" of type "{}"; {})"sv,
				a_definitionFilePath.generic_string(), a_cachedFileInfo->fileType, cacheErrorMessage.value());
		}

		return std::nullopt;
	}
}
