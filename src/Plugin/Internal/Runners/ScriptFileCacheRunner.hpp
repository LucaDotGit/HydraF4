#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::IO
{
	class IOSandbox;
}

namespace Plugin::Internal::Script
{
	class ScriptVariantMap;
}

namespace Plugin::Internal::Runners
{
	class ScriptFileCacheRunner final
		: public IFileRunner
	{
	public:
		class CachedFileInfo final
			: public std::enable_shared_from_this<CachedFileInfo>
		{
		public:
			CachedFileInfo() = default;
			~CachedFileInfo() noexcept = default;

			CachedFileInfo(const CachedFileInfo&) = default;
			CachedFileInfo(CachedFileInfo&&) noexcept = default;

			CachedFileInfo& operator=(const CachedFileInfo&) = default;
			CachedFileInfo& operator=(CachedFileInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<CachedFileInfo, std::string>;

			std::string fileType;
			std::filesystem::path directoryPath;
			std::filesystem::path fileNamePattern;
			std::string mapNamespace;
			std::optional<bool> searchRecursively;
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

			std::vector<REX::NotNull<std::shared_ptr<CachedFileInfo>>> cachedFiles;
		};

		ScriptFileCacheRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<IO::IOSandbox>>& a_ioSandbox,
			const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap);

		~ScriptFileCacheRunner() noexcept override;

		ScriptFileCacheRunner(const ScriptFileCacheRunner&) = delete;
		ScriptFileCacheRunner(ScriptFileCacheRunner&&) = delete;

		ScriptFileCacheRunner& operator=(const ScriptFileCacheRunner&) = delete;
		ScriptFileCacheRunner& operator=(ScriptFileCacheRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		class IFileCacher
			: public std::enable_shared_from_this<IFileCacher>
		{
		public:
			IFileCacher() = default;
			virtual ~IFileCacher() noexcept = default;

			IFileCacher(const IFileCacher&) = delete;
			IFileCacher(IFileCacher&&) = delete;

			IFileCacher& operator=(const IFileCacher&) = delete;
			IFileCacher& operator=(IFileCacher&&) = delete;

			virtual auto CacheFile(
				const std::filesystem::path& a_definitionFilePath,
				const RE::BSFixedString& a_mapNamespace,
				const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
				-> std::optional<std::string> = 0;
		};

		class TextFileCacher final
			: public IFileCacher
		{
		public:
			TextFileCacher() = default;
			~TextFileCacher() noexcept override = default;

			TextFileCacher(const TextFileCacher&) = delete;
			TextFileCacher(TextFileCacher&&) = delete;

			TextFileCacher& operator=(const TextFileCacher&) = delete;
			TextFileCacher& operator=(TextFileCacher&&) = delete;

			auto CacheFile(
				const std::filesystem::path& a_definitionFilePath,
				const RE::BSFixedString& a_mapNamespace,
				const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
				-> std::optional<std::string> override;
		};

		class IniFileCacher final
			: public IFileCacher
		{
		public:
			IniFileCacher() = default;
			~IniFileCacher() noexcept override = default;

			IniFileCacher(const IniFileCacher&) = delete;
			IniFileCacher(IniFileCacher&&) = delete;

			IniFileCacher& operator=(const IniFileCacher&) = delete;
			IniFileCacher& operator=(IniFileCacher&&) = delete;

			auto CacheFile(
				const std::filesystem::path& a_definitionFilePath,
				const RE::BSFixedString& a_mapNamespace,
				const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
				-> std::optional<std::string> override;
		};

		class TomlFileCacher final
			: public IFileCacher
		{
		public:
			TomlFileCacher() = default;
			~TomlFileCacher() noexcept override = default;

			TomlFileCacher(const TomlFileCacher&) = delete;
			TomlFileCacher(TomlFileCacher&&) = delete;

			TomlFileCacher& operator=(const TomlFileCacher&) = delete;
			TomlFileCacher& operator=(TomlFileCacher&&) = delete;

			auto CacheFile(
				const std::filesystem::path& a_definitionFilePath,
				const RE::BSFixedString& a_mapNamespace,
				const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
				-> std::optional<std::string> override;
		};

		class JsonFileCacher final
			: public IFileCacher
		{
		public:
			JsonFileCacher() = default;
			~JsonFileCacher() noexcept override = default;

			JsonFileCacher(const JsonFileCacher&) = delete;
			JsonFileCacher(JsonFileCacher&&) = delete;

			JsonFileCacher& operator=(const JsonFileCacher&) = delete;
			JsonFileCacher& operator=(JsonFileCacher&&) = delete;

			auto CacheFile(
				const std::filesystem::path& a_definitionFilePath,
				const RE::BSFixedString& a_mapNamespace,
				const REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>>& a_scriptVariantMap)
				-> std::optional<std::string> override;
		};

		auto RunDirectoryCaching(const REX::NotNull<std::shared_ptr<CachedFileInfo>>& a_cachedFileInfo)
			-> std::vector<std::string>;

		auto RunFileCaching(
			const REX::NotNull<std::shared_ptr<CachedFileInfo>>& a_cachedFileInfo,
			const std::filesystem::path& a_definitionFilePath)
			-> std::optional<std::string>;

		REX::NotNull<std::shared_ptr<IO::IOSandbox>> _ioSandbox;
		REX::NotNull<std::shared_ptr<Script::ScriptVariantMap>> _scriptVariantMapCacher;
		std::unordered_map<std::string_view, REX::NotNull<std::shared_ptr<IFileCacher>>> _fileCachers;
		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
