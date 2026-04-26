#pragma once

#include "Plugin/Internal/Core/JsonUtility.hpp"

namespace Plugin::Internal::Script
{
	class ScriptVariantMap;
}

namespace Plugin::Internal::IO
{
	class IOMutex;

	class JsonFile final
	{
	public:
		using json_t = Core::JsonUtility::json_t;

		JsonFile(const REX::NotNull<std::shared_ptr<IO::IOMutex>>& a_ioMutex);
		~JsonFile() noexcept;

		JsonFile(const JsonFile&) = delete;
		JsonFile(JsonFile&&) = delete;

		JsonFile& operator=(const JsonFile&) = delete;
		JsonFile& operator=(JsonFile&&) = delete;

		bool ReadFile(const std::filesystem::path& a_path, json_t& a_root) const;
		bool WriteFile(const std::filesystem::path& a_path, const json_t& a_root) const;

		[[nodiscard]] auto Deserialize(const std::filesystem::path& a_path, const RE::BSFixedString& a_structName) const
			-> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>;
		bool Serialize(const std::filesystem::path& a_path, const RE::BSTSmartPointer<RE::BSScript::Struct>& a_struct) const;

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
