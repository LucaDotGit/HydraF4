#include "Plugin/Internal/Script/ScriptVariantSet.hpp"

namespace Plugin::Internal::Script
{
	bool ScriptVariantSet::IsNamespaceValid([[maybe_unused]] const RE::BSFixedString& a_namespace) const
	{
		return true;
	}

	bool ScriptVariantSet::AddValidNamespace([[maybe_unused]] const RE::BSFixedString& a_namespace)
	{
		return false;
	}

	auto ScriptVariantSet::GetNamespaces() const -> std::vector<RE::BSFixedString>
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);

		auto names = std::vector<RE::BSFixedString>();
		names.reserve(_variantSet.size());

		for (const auto& [name, _] : _variantSet) {
			names.push_back(name);
		}

		return names;
	}

	auto ScriptVariantSet::GetSortedNamespaces() const -> std::vector<RE::BSFixedString>
	{
		auto names = GetNamespaces();
		std::ranges::sort(names);
		return names;
	}

	auto ScriptVariantSet::GetKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);

		const auto nameIt = _variantSet.find(a_namespace);
		if (nameIt == _variantSet.end()) {
			return {};
		}

		auto keys = std::vector<RE::BSScript::UniqueVariant>();
		keys.reserve(nameIt->second.size());

		for (const auto& key : nameIt->second) {
			auto variant = RE::BSScript::CreateVariant(key);
			keys.push_back(std::move(variant));
		}

		return keys;
	}

	auto ScriptVariantSet::GetSortedKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>
	{
		auto keys = GetKeys(a_namespace);
		std::ranges::sort(keys);
		return keys;
	}

	auto ScriptVariantSet::GetNamespaceSize() const -> std::size_t
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);
		return _variantSet.size();
	}

	bool ScriptVariantSet::ContainsNamespace(const RE::BSFixedString& a_namespace) const
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);
		return _variantSet.contains(a_namespace);
	}

	bool ScriptVariantSet::ContainsKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) const
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);

		const auto nameIt = _variantSet.find(a_namespace);
		if (nameIt == _variantSet.end()) {
			return false;
		}

		return nameIt->second.contains(RE::BSScript::UnwrapVariant(a_key));
	}

	bool ScriptVariantSet::Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key)
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		auto& keys = _variantSet[a_namespace];
		const auto&& [_, hasInserted] = keys.insert(RE::BSScript::UnwrapVariant(a_key));
		return hasInserted;
	}

	bool ScriptVariantSet::AddRange(const RE::BSFixedString& a_namespace, std::span<const RE::BSScript::SharedVariant> a_keys)
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		auto& keys = _variantSet[a_namespace];
		keys.reserve(keys.size() + a_keys.size());

		for (const auto& key : a_keys) {
			keys.insert(RE::BSScript::UnwrapVariant(key));
		}

		return true;
	}

	bool ScriptVariantSet::AddRange(const RE::BSFixedString& a_namespace, std::span<RE::BSScript::Variable> a_keys)
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		auto& keys = _variantSet[a_namespace];
		keys.reserve(keys.size() + a_keys.size());

		for (auto& key : a_keys) {
			keys.insert(std::move(key));
		}

		return true;
	}

	bool ScriptVariantSet::RemoveKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key)
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		const auto nameIt = _variantSet.find(a_namespace);
		if (nameIt == _variantSet.end()) {
			return false;
		}

		const auto keyIt = nameIt->second.find(RE::BSScript::UnwrapVariant(a_key));
		if (keyIt == nameIt->second.end()) {
			return false;
		}

		nameIt->second.erase(keyIt);
		return true;
	}

	bool ScriptVariantSet::RemoveNamespace(const RE::BSFixedString& a_namespace)
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		const auto nameIt = _variantSet.find(a_namespace);
		if (nameIt == _variantSet.end()) {
			return false;
		}

		_variantSet.erase(nameIt);
		return true;
	}

	void ScriptVariantSet::Clear() noexcept
	{
		const auto variantSetLock = std::scoped_lock(_variantSetMutex);

		_variantSet.clear();
		_variantSet.rehash(0);
	}

	bool ScriptVariantSet::ForEach(
		const REX::NotNull<std::function<bool(const RE::BSFixedString&, const RE::BSScript::Variable&)>>& a_predicate) const
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);

		for (const auto& [name, keys] : _variantSet) {
			const auto allTrue = std::ranges::all_of(keys, [&a_predicate, &name](const RE::BSScript::Variable& a_key) {
				return std::invoke(*a_predicate, name, a_key);
			});

			if (!allTrue) {
				return false;
			}
		}

		return true;
	}

	bool ScriptVariantSet::ForEach(const RE::BSFixedString& a_namespace,
		const REX::NotNull<std::function<bool(const RE::BSScript::Variable&)>>& a_predicate) const
	{
		const auto variantSetLock = std::shared_lock(_variantSetMutex);

		const auto nameIt = _variantSet.find(a_namespace);
		if (nameIt == _variantSet.end()) {
			return false;
		}

		return std::ranges::all_of(nameIt->second, [&a_predicate](const RE::BSScript::Variable& a_key) {
			return std::invoke(*a_predicate, a_key);
		});
	}
}
