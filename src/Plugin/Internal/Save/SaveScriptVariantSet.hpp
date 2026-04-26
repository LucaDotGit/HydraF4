#pragma once

#include "Plugin/Internal/Save/ISaveScriptVariantCollection.hpp"
#include "Plugin/Internal/Script/ScriptVariantSet.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptVariantSet final
		: public ISaveScriptVariantCollection,
		  public Script::ScriptVariantSet
	{
	public:
		SaveScriptVariantSet(std::uint32_t a_type, std::uint32_t a_version);
		~SaveScriptVariantSet() noexcept override;

		SaveScriptVariantSet(const SaveScriptVariantSet&) = delete;
		SaveScriptVariantSet(SaveScriptVariantSet&&) = delete;

		SaveScriptVariantSet& operator=(const SaveScriptVariantSet&) = delete;
		SaveScriptVariantSet& operator=(SaveScriptVariantSet&&) = delete;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

		bool IsNamespaceValid(const RE::BSFixedString& a_namespace) const override;
		bool AddValidNamespace(const RE::BSFixedString& a_namespace) override;

		bool Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key) override;

		bool AddRange(const RE::BSFixedString& a_namespace, std::span<const RE::BSScript::SharedVariant> a_keys) override;
		bool AddRange(const RE::BSFixedString& a_namespace, std::span<RE::BSScript::Variable> a_keys) override;
	};
}
