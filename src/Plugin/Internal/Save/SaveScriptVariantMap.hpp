#pragma once

#include "Plugin/Internal/Save/ISaveScriptVariantCollection.hpp"
#include "Plugin/Internal/Script/ScriptVariantMap.hpp"
#include "Plugin/Internal/Structs/VarPairs.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptVariantMap final
		: public ISaveScriptVariantCollection,
		  public Script::ScriptVariantMap
	{
	public:
		SaveScriptVariantMap(std::uint32_t a_type, std::uint32_t a_version);
		~SaveScriptVariantMap() noexcept override;

		SaveScriptVariantMap(const SaveScriptVariantMap&) = delete;
		SaveScriptVariantMap(SaveScriptVariantMap&&) = delete;

		SaveScriptVariantMap& operator=(const SaveScriptVariantMap&) = delete;
		SaveScriptVariantMap& operator=(SaveScriptVariantMap&&) = delete;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

		bool IsNamespaceValid(const RE::BSFixedString& a_namespace) const override;
		bool AddValidNamespace(const RE::BSFixedString& a_namespace) override;

		bool SetPairs(const RE::BSFixedString& a_namespace, std::span<const std::optional<Structs::VarPairs::VarPairData>> a_pairs) override;
		bool SetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value) override;

		bool SetRange(const RE::BSFixedString& a_namespace,
			std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs) override;
		bool SetRange(const RE::BSFixedString& a_namespace,
			std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs) override;

		bool Update(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_newValue, RE::BSScript::SharedVariant a_compareValue) override;
		bool Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value) override;

		bool AddRange(const RE::BSFixedString& a_namespace,
			std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs) override;
		bool AddRange(const RE::BSFixedString& a_namespace,
			std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs) override;

		auto AddOrUpdate(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_addvalue, RE::BSScript::SharedVariant a_updateValue)
			-> RE::BSScript::UniqueVariant override;
		auto GetOrAdd(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
			RE::BSScript::SharedVariant a_value) -> RE::BSScript::UniqueVariant override;
	};
}
