#pragma once

namespace Plugin::Internal::Script
{
	class ScriptVariantSet
	{
	public:
		ScriptVariantSet() = default;
		virtual ~ScriptVariantSet() noexcept = default;

		ScriptVariantSet(const ScriptVariantSet&) = delete;
		ScriptVariantSet(ScriptVariantSet&&) = delete;

		ScriptVariantSet& operator=(const ScriptVariantSet&) = delete;
		ScriptVariantSet& operator=(ScriptVariantSet&&) = delete;

		[[nodiscard]] virtual bool IsNamespaceValid(const RE::BSFixedString& a_namespace) const;
		virtual bool AddValidNamespace(const RE::BSFixedString& a_namespace);

		[[nodiscard]] virtual auto GetNamespaces() const -> std::vector<RE::BSFixedString>;
		[[nodiscard]] virtual auto GetSortedNamespaces() const -> std::vector<RE::BSFixedString>;

		[[nodiscard]] virtual auto GetKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;
		[[nodiscard]] virtual auto GetSortedKeys(const RE::BSFixedString& a_namespace) const -> std::vector<RE::BSScript::UniqueVariant>;

		[[nodiscard]] virtual std::size_t GetNamespaceSize() const;

		[[nodiscard]] virtual bool ContainsNamespace(const RE::BSFixedString& a_namespace) const;
		[[nodiscard]] virtual bool ContainsKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) const;

		virtual bool Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key);

		virtual bool AddRange(const RE::BSFixedString& a_namespace, std::span<const RE::BSScript::SharedVariant> a_keys);
		virtual bool AddRange(const RE::BSFixedString& a_namespace, std::span<RE::BSScript::Variable> a_keys);

		virtual bool RemoveKey(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key);
		virtual bool RemoveNamespace(const RE::BSFixedString& a_namespace);

		virtual void Clear() noexcept;

		virtual bool ForEach(
			const REX::NotNull<std::function<bool(const RE::BSFixedString&, const RE::BSScript::Variable&)>>& a_predicate) const;
		virtual bool ForEach(const RE::BSFixedString& a_namespace,
			const REX::NotNull<std::function<bool(const RE::BSScript::Variable&)>>& a_predicate) const;

	protected:
		mutable std::shared_mutex _variantSetMutex;
		std::unordered_map<RE::BSFixedString, std::unordered_set<RE::BSScript::Variable>> _variantSet;
	};
}
