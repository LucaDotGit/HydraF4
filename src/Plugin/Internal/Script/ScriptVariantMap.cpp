#include "Plugin/Internal/Script/ScriptVariantMap.hpp"

namespace Plugin::Internal::Script
{
	bool ScriptVariantMap::IsNamespaceValid([[maybe_unused]] const RE::BSFixedString& a_namespace) const
	{
		return true;
	}

	bool ScriptVariantMap::AddValidNamespace([[maybe_unused]] const RE::BSFixedString& a_namespace)
	{
		return false;
	}

	auto ScriptVariantMap::GetNamespaces() const -> std::vector<RE::BSFixedString>
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		auto names = std::vector<RE::BSFixedString>();
		names.reserve(_variantMap.size());

		for (const auto& [name, _] : _variantMap) {
			names.push_back(name);
		}

		return names;
	}

	auto ScriptVariantMap::GetSortedNamespaces() const -> std::vector<RE::BSFixedString>
	{
		auto names = GetNamespaces();
		std::ranges::sort(names);
		return names;
	}

	auto ScriptVariantMap::GetKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return {};
		}

		auto keys = std::vector<RE::BSScript::UniqueVariant>();
		keys.reserve(nameIt->second.size());

		for (const auto& [key, _] : nameIt->second) {
			auto variant = RE::BSScript::CreateVariant(key);
			keys.push_back(std::move(variant));
		}

		return keys;
	}

	auto ScriptVariantMap::GetSortedKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		auto keys = GetKeys(a_namespace);
		std::ranges::sort(keys);
		return keys;
	}

	auto ScriptVariantMap::GetValues(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return {};
		}

		auto values = std::vector<RE::BSScript::UniqueVariant>();
		values.reserve(nameIt->second.size());

		for (const auto& [_, value] : nameIt->second) {
			auto variant = RE::BSScript::CreateVariant(value);
			values.push_back(std::move(variant));
		}

		return values;
	}

	auto ScriptVariantMap::GetSortedValues(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		auto values = GetValues(a_namespace);
		std::ranges::sort(values);
		return values;
	}

	auto ScriptVariantMap::GetPairs(const RE::BSFixedString& a_namespace) const -> std::vector<std::optional<Structs::VarPairs::VarPairData>>
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return {};
		}

		auto pairs = std::vector<std::optional<Structs::VarPairs::VarPairData>>();
		pairs.reserve(nameIt->second.size());

		for (const auto& [key, value] : nameIt->second) {
			auto pair = std::make_pair(key, value);
			pairs.emplace_back(std::move(pair));
		}

		return pairs;
	}

	auto ScriptVariantMap::GetSortedPairs(const RE::BSFixedString& a_namespace) const -> std::vector<std::optional<Structs::VarPairs::VarPairData>>
	{
		auto pairs = GetPairs(a_namespace);

		std::ranges::sort(pairs, [](const std::optional<Structs::VarPairs::VarPairData>& a_lhs, const std::optional<Structs::VarPairs::VarPairData>& a_rhs) {
			if (!a_lhs.has_value()) {
				return true;
			}

			if (!a_rhs.has_value()) {
				return false;
			}

			return a_lhs->first < a_rhs->first;
		});

		return pairs;
	}

	bool ScriptVariantMap::SetPairs(const RE::BSFixedString& a_namespace, std::span<const std::optional<Structs::VarPairs::VarPairData>> a_pairs)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];

		for (const auto& pair : a_pairs) {
			if (!pair.has_value()) {
				continue;
			}

			keys.insert_or_assign(pair->first, pair->second);
		}

		return true;
	}

	std::size_t ScriptVariantMap::GetNamespaceSize() const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);
		return _variantMap.size();
	}

	std::size_t ScriptVariantMap::GetKeySize(const RE::BSFixedString& a_namespace) const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return 0;
		}

		return nameIt->second.size();
	}

	bool ScriptVariantMap::ContainsNamespace(const RE::BSFixedString& a_namespace) const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);
		return _variantMap.contains(a_namespace);
	}

	bool ScriptVariantMap::ContainsKey(const RE::BSFixedString& a_namespace,
		RE::BSScript::SharedVariant a_key) const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return false;
		}

		return nameIt->second.contains(RE::BSScript::UnwrapVariant(a_key));
	}

	auto ScriptVariantMap::GetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) const
		-> RE::BSScript::UniqueVariant
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return nullptr;
		}

		const auto keyIt = nameIt->second.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == nameIt->second.end()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(keyIt->second);
	}

	auto ScriptVariantMap::GetValueOrDefault(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_default) const
		-> RE::BSScript::UniqueVariant
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return RE::BSScript::CreateVariant(a_default);
		}

		const auto keyIt = nameIt->second.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == nameIt->second.end()) {
			return RE::BSScript::CreateVariant(a_default);
		}

		return RE::BSScript::CreateVariant(keyIt->second);
	}

	bool ScriptVariantMap::SetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		_variantMap[a_namespace].insert_or_assign(RE::BSScript::UnwrapVariant(a_key), RE::BSScript::UnwrapVariant(a_value));
		return true;
	}

	bool ScriptVariantMap::SetRange(const RE::BSFixedString& a_namespace,
		std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];
		keys.reserve(keys.size() + a_pairs.size());

		for (const auto& [key, value] : a_pairs) {
			keys.insert_or_assign(RE::BSScript::UnwrapVariant(key), RE::BSScript::UnwrapVariant(value));
		}

		return true;
	}

	bool ScriptVariantMap::SetRange(const RE::BSFixedString& a_namespace,
		std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];
		keys.reserve(keys.size() + a_pairs.size());

		for (auto& [key, value] : a_pairs) {
			keys.insert_or_assign(std::move(key), std::move(value));
		}

		return true;
	}

	bool ScriptVariantMap::Update(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_newValue, RE::BSScript::SharedVariant a_compareValue)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return false;
		}

		auto& keys = nameIt->second;

		const auto keyIt = keys.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == keys.end()) {
			return false;
		}

		if (!keyIt->second.Equals(RE::BSScript::UnwrapVariant(a_compareValue))) {
			return false;
		}

		keyIt->second = RE::BSScript::UnwrapVariant(a_newValue);
		return true;
	}

	bool ScriptVariantMap::Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];
		const auto&& [_, hasInserted] = keys.emplace(RE::BSScript::UnwrapVariant(a_key), RE::BSScript::UnwrapVariant(a_value));
		return hasInserted;
	}

	bool ScriptVariantMap::AddRange(const RE::BSFixedString& a_namespace,
		std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];
		keys.reserve(keys.size() + a_pairs.size());

		for (const auto& [key, value] : a_pairs) {
			keys.emplace(RE::BSScript::UnwrapVariant(key), RE::BSScript::UnwrapVariant(value));
		}

		return true;
	}

	bool ScriptVariantMap::AddRange(const RE::BSFixedString& a_namespace,
		std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];
		keys.reserve(keys.size() + a_pairs.size());

		for (auto& [key, value] : a_pairs) {
			keys.emplace(std::move(key), std::move(value));
		}

		return true;
	}

	auto ScriptVariantMap::AddOrUpdate(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_addValue, RE::BSScript::SharedVariant a_updateValue)
		-> RE::BSScript::UniqueVariant
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];

		const auto keyIt = keys.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == keys.end()) {
			keys.insert_or_assign(RE::BSScript::UnwrapVariant(a_key), RE::BSScript::UnwrapVariant(a_addValue));
			return RE::BSScript::CreateVariant(a_addValue);
		}

		keyIt->second = RE::BSScript::UnwrapVariant(a_updateValue);
		return RE::BSScript::CreateVariant(a_updateValue);
	}

	auto ScriptVariantMap::GetOrAdd(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value) -> RE::BSScript::UniqueVariant
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		auto& keys = _variantMap[a_namespace];

		const auto keyIt = keys.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt != keys.end()) {
			return RE::BSScript::CreateVariant(keyIt->second);
		}

		keys.insert_or_assign(RE::BSScript::UnwrapVariant(a_key), RE::BSScript::UnwrapVariant(a_value));
		return RE::BSScript::CreateVariant(a_value);
	}

	bool ScriptVariantMap::RemoveKey(const RE::BSFixedString& a_namespace,
		RE::BSScript::SharedVariant a_key)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return false;
		}

		const auto keyIt = nameIt->second.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == nameIt->second.end()) {
			return false;
		}

		nameIt->second.erase(keyIt);
		return true;
	}

	bool ScriptVariantMap::RemoveNamespace(const RE::BSFixedString& a_namespace)
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return false;
		}

		_variantMap.erase(nameIt);
		return true;
	}

	void ScriptVariantMap::Clear() noexcept
	{
		const auto variantMapLock = std::scoped_lock(_variantMapMutex);

		_variantMap.clear();
		_variantMap.rehash(0);
	}

	bool ScriptVariantMap::ForEach(
		const REX::NotNull<std::function<bool(const RE::BSFixedString&, const RE::BSScript::Variable&, const RE::BSScript::Variable&)>>& a_predicate) const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		for (const auto& [name, keys] : _variantMap) {
			const auto allTrue = std::ranges::all_of(keys, [&a_predicate, &name](const std::pair<const RE::BSScript::Variable, RE::BSScript::Variable>& a_pair) {
				return std::invoke(*a_predicate, name, a_pair.first, a_pair.second);
			});

			if (!allTrue) {
				return false;
			}
		}

		return true;
	}

	bool ScriptVariantMap::ForEach(const RE::BSFixedString& a_namespace,
		const REX::NotNull<std::function<bool(const RE::BSScript::Variable&, const RE::BSScript::Variable&)>>& a_predicate) const
	{
		const auto variantMapLock = std::shared_lock(_variantMapMutex);

		const auto nameIt = _variantMap.find(a_namespace);
		if (nameIt == _variantMap.end()) {
			return false;
		}

		return std::ranges::all_of(nameIt->second, [&a_predicate](const std::pair<const RE::BSScript::Variable, RE::BSScript::Variable>& a_pair) {
			return std::invoke(*a_predicate, a_pair.first, a_pair.second);
		});
	}
}
