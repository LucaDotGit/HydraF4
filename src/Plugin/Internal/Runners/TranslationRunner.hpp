#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	class TranslationRunner final
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

			std::vector<std::pair<std::wstring, std::wstring>> translations;
		};

		TranslationRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		~TranslationRunner() noexcept override;

		TranslationRunner(const TranslationRunner&) = delete;
		TranslationRunner(TranslationRunner&&) = delete;

		TranslationRunner& operator=(const TranslationRunner&) = delete;
		TranslationRunner& operator=(TranslationRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		static auto RunTranslation(
			F4SE::Translations::Translator& a_gameTranslations,
			const std::pair<std::wstring, std::wstring>& a_translationPair)
			-> std::optional<std::wstring>;
		static void RunTranslations(
			F4SE::Translations::Translator& a_gameTranslations,
			const std::filesystem::path& a_filePath,
			const REX::NotNull<std::shared_ptr<RootInfo>>& a_rootInfo);

		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
