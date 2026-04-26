#include "Plugin/Internal/Script/ScriptStrictStringPool.hpp"

namespace Plugin::Internal::Script
{
	bool ScriptStrictStringPool::Contains(const RE::BSFixedString& a_key) const noexcept
	{
		if (!IsKeyValid(static_cast<std::string_view>(a_key))) {
			return false;
		}

		const auto poolLock = std::shared_lock(_mapMutex);
		return _stringMap.contains(a_key);
	}

	auto ScriptStrictStringPool::GetValue(const RE::BSFixedString& a_key) const noexcept -> std::optional<RE::BSFixedStringCS>
	{
		if (!IsKeyValid(static_cast<std::string_view>(a_key))) {
			return std::nullopt;
		}

		const auto poolLock = std::shared_lock(_mapMutex);

		const auto stringIt = _stringMap.find(a_key);
		if (stringIt == _stringMap.end()) {
			return std::nullopt;
		}

		return stringIt->second;
	}

	void ScriptStrictStringPool::SetValue(const RE::BSFixedString& a_key, const RE::BSFixedStringCS& a_value)
	{
		if (!IsKeyValid(static_cast<std::string_view>(a_key))) {
			return;
		}

		const auto poolLock = std::scoped_lock(_mapMutex);
		_stringMap[a_key] = a_value;
	}

	bool ScriptStrictStringPool::AddValue(const RE::BSFixedString& a_key, const RE::BSFixedStringCS& a_value)
	{
		if (!IsKeyValid(static_cast<std::string_view>(a_key))) {
			return false;
		}

		const auto poolLock = std::scoped_lock(_mapMutex);

		const auto&& [_, hasInserted] = _stringMap.emplace(a_key, a_value);
		return hasInserted;
	}

	bool ScriptStrictStringPool::Remove(const RE::BSFixedString& a_key) noexcept
	{
		if (!IsKeyValid(static_cast<std::string_view>(a_key))) {
			return false;
		}

		const auto poolLock = std::scoped_lock(_mapMutex);

		const auto stringIt = _stringMap.find(a_key);
		if (stringIt == _stringMap.end()) {
			return false;
		}

		_stringMap.erase(stringIt);
		return true;
	}

	void ScriptStrictStringPool::Clear() noexcept
	{
		const auto poolLock = std::scoped_lock(_mapMutex);

		_stringMap.clear();
		_stringMap.rehash(0);
	}

	auto GetScriptStrictStringPool() noexcept -> const REX::NotNull<std::unique_ptr<ScriptStrictStringPool>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<ScriptStrictStringPool>());
		return INSTANCE;
	}
}
