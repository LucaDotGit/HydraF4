#pragma once

#include "Plugin/Internal/Structs/VarPairs.hpp"

namespace Plugin::Internal::Script
{
	class ScriptVariantMap
	{
	public:
		ScriptVariantMap() = default;
		virtual ~ScriptVariantMap() noexcept = default;

		ScriptVariantMap(const ScriptVariantMap&) = delete;
		ScriptVariantMap(ScriptVariantMap&&) = delete;

		ScriptVariantMap& operator=(const ScriptVariantMap&) = delete;
		ScriptVariantMap& operator=(ScriptVariantMap&&) = delete;

		[[nodiscard]] virtual bool IsNamespaceValid(const RE::BSFixedString& a_namespace) const;
		virtual bool AddValidNamespace(const RE::BSFixedString& a_namespace);

		[[nodiscard]] virtual auto GetNamespaces() const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] virtual auto GetSortedNamespaces() const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] virtual auto GetKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;
		[[nodiscard]] virtual auto GetSortedKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;

		[[nodiscard]] virtual auto GetValues(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;
		[[nodiscard]] virtual auto GetSortedValues(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;

		[[nodiscard]] virtual auto GetPairs(const RE::BSFixedString& a_namespace) const -> std::vector<std::optional<Structs::VarPairs::VarPairData>>;
		[[nodiscard]] virtual auto GetSortedPairs(const RE::BSFixedString& a_namespace) const -> std::vector<std::optional<Structs::VarPairs::VarPairData>>;
		virtual bool SetPairs(const RE::BSFixedString& a_namespace, std::span<const std::optional<Structs::VarPairs::VarPairData>> a_pairs);

		[[nodiscard]] virtual std::size_t GetNamespaceSize() const;
		[[nodiscard]] virtual std::size_t GetKeySize(const RE::BSFixedString& a_namespace) const;

		[[nodiscard]] virtual bool ContainsNamespace(const RE::BSFixedString& a_namespace) const;
		[[nodiscard]] virtual bool ContainsKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) const;

		[[nodiscard]] virtual auto GetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) const
			-> RE::BSScript::UniqueVariant;
		[[nodiscard]] virtual auto GetValueOrDefault(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_default) const -> RE::BSScript::UniqueVariant;

		virtual bool SetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value);

		virtual bool SetRange(const RE::BSFixedString& a_namespace,
			std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs);
		virtual bool SetRange(const RE::BSFixedString& a_namespace,
			std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs);

		virtual bool Update(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_newValue, RE::BSScript::SharedVariant a_compareValue);
		virtual bool Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value);

		virtual bool AddRange(const RE::BSFixedString& a_namespace,
			std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs);
		virtual bool AddRange(const RE::BSFixedString& a_namespace,
			std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs);

		virtual auto AddOrUpdate(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_addValue, RE::BSScript::SharedVariant a_updateValue)
			-> RE::BSScript::UniqueVariant;
		virtual auto GetOrAdd(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value) -> RE::BSScript::UniqueVariant;

		virtual bool RemoveKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key);
		virtual bool RemoveNamespace(const RE::BSFixedString& a_namespace);

		virtual void Clear() noexcept;

		virtual bool ForEach(
			const REX::NotNull<std::function<bool(const RE::BSFixedString&, const RE::BSScript::Variable&, const RE::BSScript::Variable&)>>& a_predicate) const;
		virtual bool ForEach(const RE::BSFixedString& a_namespace,
			const REX::NotNull<std::function<bool(const RE::BSScript::Variable&, const RE::BSScript::Variable&)>>& a_predicate) const;

	protected:
		mutable std::shared_mutex _variantMapMutex;
		std::unordered_map<RE::BSFixedString, std::unordered_map<RE::BSScript::Variable, RE::BSScript::Variable>> _variantMap;
	};
}
