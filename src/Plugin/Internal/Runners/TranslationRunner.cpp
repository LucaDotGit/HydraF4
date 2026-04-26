#include "Plugin/Internal/Runners/TranslationRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Runners
{
	auto TranslationRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		auto rootInfo = RootInfo();

		auto& translations = rootInfo.translations;
		translations.reserve(a_json.size());

		for (const auto& [key, node] : a_json.items()) {
			if (!node.is_string()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find string value for key "{}")"sv,
					key);
			}

			auto u16Key = REX::Utf8ToUtf16(key);
			if (!u16Key.has_value()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to convert key to UTF-16: "{}" ({}))"sv,
					key, u16Key.error());
			}

			const auto u8Value = node.get<std::string>();

			auto u16Value = REX::Utf8ToUtf16(u8Value);
			if (!u16Value.has_value()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to convert value to UTF-16: "{}" ({}))"sv,
					node.get<std::string>(), u16Value.error());
			}

			translations.emplace_back(std::move(u16Key).value(), std::move(u16Value).value());
		}

		return rootInfo;
	}

	TranslationRunner::TranslationRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension))
	{
	}

	TranslationRunner::~TranslationRunner() noexcept
	{
		Dispose();
	}

	auto TranslationRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto languageCode = F4SE::Translations::GetTranslator()->GetLanguageCode();
		if (languageCode.empty()) [[unlikely]] {
			REX::Fail("Failed to get the game's language code."sv);
		}

		auto filePaths = IFileRunner::LoadDirectoryFiles();
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			REX::Fail(R"(Failed to iterate directory "{}": {})"sv,
				_directoryPath.generic_string(), filePaths.error());
		}

		return a_threadPool.SubmitTaskRange(std::move(filePaths).value(), [this, languageCode](const std::filesystem::path& a_filePath) {
			const auto stemName = a_filePath.stem().generic_string();

			const auto languageIt = stemName.find_last_of(RE::BSScaleformTranslator::TRANSLATION_LANGUAGE_SEPARATOR<char>);
			if (languageIt == std::string::npos) [[unlikely]] {
				REX::LogError(R"(Failed to find language separator "{}" in file "{}")"sv,
					RE::BSScaleformTranslator::TRANSLATION_LANGUAGE_SEPARATOR<char>, a_filePath.generic_string());
				return;
			}

			const auto fileLanguage = std::string_view{ stemName.data() + languageIt + 1, stemName.size() - languageIt - 1 };
			if (!REX::EqualsIgnoreCase(fileLanguage, languageCode) && !REX::EqualsIgnoreCase(fileLanguage, RE::BSScaleformTranslator::DEFAULT_LANGUAGE)) {
				REX::LogDebug(R"(Skipping file "{}" with language "{}"; current language is "{}")"sv,
					a_filePath.generic_string(), fileLanguage, languageCode);
				return;
			}

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

			REX::LogDebug(R"(Loaded file "{}" with {} translation(s))"sv,
				a_filePath.generic_string(), rootInfo->translations.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto TranslationRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto& translations = F4SE::Translations::GetTranslator();

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			RunTranslations(*(translations.get()), filePath, rootInfo);
		}

		return {};
	}

	void TranslationRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto TranslationRunner::RunTranslation(
		F4SE::Translations::Translator& a_gameTranslations,
		const std::pair<std::wstring, std::wstring>& a_translationPair)
		-> std::optional<std::wstring>
	{
		const auto& [key, value] = a_translationPair;

		if (!RE::BSScaleformTranslator::IsKeyValid(static_cast<std::wstring_view>(key))) [[unlikely]] {
			return REX::Format(LR"(invalid or missing key prefix ({}) for key "{}")"sv,
				RE::BSScaleformTranslator::TRANSLATION_KEY_PREFIX<wchar_t>, key);
		}

		const auto nativeKey = RE::BSFixedStringWCS(key);
		const auto nativeValue = RE::BSFixedStringWCS(value);

		a_gameTranslations.SetValue(nativeKey, nativeValue);
		return std::nullopt;
	}

	void TranslationRunner::RunTranslations(
		F4SE::Translations::Translator& a_gameTranslations,
		const std::filesystem::path& a_filePath,
		const REX::NotNull<std::shared_ptr<RootInfo>>& a_rootInfo)
	{
		const auto& translationMap = a_rootInfo->translations;
		a_gameTranslations.Reserve(a_gameTranslations.GetSize() + static_cast<std::uint32_t>(translationMap.size()));

		for (const auto& translationPair : translationMap) {
			auto runTranslationResult = RunTranslation(a_gameTranslations, translationPair);
			if (runTranslationResult.has_value()) [[unlikely]] {
				REX::LogError(LR"(Error in file "{}"; {})"sv,
					a_filePath.generic_wstring(), runTranslationResult.value());
				continue;
			}

			REX::LogTrace(LR"(Added translation for key "{}" from file "{}")"sv,
				translationPair.first, a_filePath.generic_wstring());
		}
	}
}
