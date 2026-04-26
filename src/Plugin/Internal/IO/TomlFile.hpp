#pragma once

#include "Plugin/Internal/Core/TomlUtility.hpp"

namespace Plugin::Internal::Script
{
	class ScriptVariantMap;
}

namespace Plugin::Internal::IO
{
	class IOMutex;

	class TomlFile final
	{
	public:
		using toml_t = Core::TomlUtility::toml_t;

		TomlFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex);
		~TomlFile() noexcept;

		TomlFile(const TomlFile&) = delete;
		TomlFile(TomlFile&&) = delete;

		TomlFile& operator=(const TomlFile&) = delete;
		TomlFile& operator=(TomlFile&&) = delete;

		bool ReadFile(const std::filesystem::path& a_path, toml_t& a_root) const;
		bool WriteFile(const std::filesystem::path& a_path, const toml_t& a_root) const;

		[[nodiscard]] static bool IsCachedAsVariantMap(const Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path);
		bool CacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const;
		bool CacheAsVariantMapTo(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_sourcePath, const RE::BSFixedString& a_targetNamespace) const;
		bool UncacheAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const;
		bool SaveCachedAsVariantMap(Script::ScriptVariantMap& a_map, const std::filesystem::path& a_path) const;
		bool SaveCachedAsVariantMapTo(Script::ScriptVariantMap& a_map, const RE::BSFixedString& a_sourceNamespace, const std::filesystem::path& a_targetPath) const;

	private:
		REX::NotNull<std::shared_ptr<IO::IOMutex>> _ioMutex;
	};
}
