#pragma once

#include "Plugin/Internal/Core/IniUtility.hpp"

namespace Plugin::Internal::Script
{
	class ScriptVariantMap;
}

namespace Plugin::Internal::IO
{
	class IOMutex;

	class IniFile final
	{
	public:
		using ini_t = Core::IniUtility::ini_t;

		IniFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex);
		~IniFile() noexcept;

		IniFile(const IniFile&) = delete;
		IniFile(IniFile&&) = delete;

		IniFile& operator=(const IniFile&) = delete;
		IniFile& operator=(IniFile&&) = delete;

		static void SetupFile(ini_t& a_file);

		bool ReadFile(const std::filesystem::path& a_path, ini_t& a_ini) const;
		bool WriteFile(const std::filesystem::path& a_path, const ini_t& a_ini) const;

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
