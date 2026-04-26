#include "Plugin/Internal/Save/ISaveScriptVariantCollection.hpp"

namespace Plugin::Internal::Save
{
	ISaveScriptVariantCollection::ISaveScriptVariantCollection(std::uint32_t a_type, std::uint32_t a_version)
		: ISaveObject(a_type, a_version)
	{
	}

	ISaveScriptVariantCollection::~ISaveScriptVariantCollection() noexcept = default;

	bool ISaveScriptVariantCollection::IsNamespaceValid(const RE::BSFixedString& a_namespace) const
	{
		const auto namespaceLock = std::shared_lock(_validNamespacesMutex);
		return _validNamespaces.contains(a_namespace);
	}

	bool ISaveScriptVariantCollection::AddValidNamespace(const RE::BSFixedString& a_namespace)
	{
		const auto namespaceLock = std::scoped_lock(_validNamespacesMutex);
		const auto&& [_, hasInserted] = _validNamespaces.emplace(a_namespace);
		return hasInserted;
	}
}
