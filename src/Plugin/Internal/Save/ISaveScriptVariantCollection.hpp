#pragma once

#include "Plugin/Internal/Save/ISaveObject.hpp"

namespace Plugin::Internal::Save
{
	class ISaveScriptVariantCollection
		: public ISaveObject
	{
	public:
		ISaveScriptVariantCollection(std::uint32_t a_type, std::uint32_t a_version);
		~ISaveScriptVariantCollection() noexcept override;

		ISaveScriptVariantCollection(const ISaveScriptVariantCollection&) = delete;
		ISaveScriptVariantCollection(ISaveScriptVariantCollection&&) = delete;

		ISaveScriptVariantCollection& operator=(const ISaveScriptVariantCollection&) = delete;
		ISaveScriptVariantCollection& operator=(ISaveScriptVariantCollection&&) = delete;

		[[nodiscard]] virtual bool IsNamespaceValid(const RE::BSFixedString& a_namespace) const;
		virtual bool AddValidNamespace(const RE::BSFixedString& a_namespace);

	protected:
		mutable std::shared_mutex _validNamespacesMutex;
		std::unordered_set<RE::BSFixedString> _validNamespaces;
	};
}
