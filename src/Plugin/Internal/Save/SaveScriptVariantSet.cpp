#include "Plugin/Internal/Save/SaveScriptVariantSet.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveScriptVariantSet::SaveScriptVariantSet(std::uint32_t a_type, std::uint32_t a_version)
		: ISaveScriptVariantCollection(a_type, a_version)
	{
	}

	SaveScriptVariantSet::~SaveScriptVariantSet() noexcept = default;

	bool SaveScriptVariantSet::Load(const SaveStream& a_stream)
	{
		const auto variableLock = std::scoped_lock(_variantSetMutex);

		auto nameCount = static_cast<std::uint32_t>(0);
		if (!a_stream.ReadValue(nameCount)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (nameCount == 0) {
			return true;
		}

		_variantSet.reserve(nameCount);

		for (auto i = static_cast<std::uint32_t>(0); i < nameCount; i++) {
			auto name = RE::BSFixedString();
			if (!a_stream.ReadString(name)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			auto keyCount = static_cast<std::uint32_t>(0);
			if (!a_stream.ReadValue(keyCount)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			const auto isNamespaceValid = IsNamespaceValid(name);

			auto& varSet = _variantSet[name];
			varSet.reserve(keyCount);

			for (auto j = static_cast<std::uint32_t>(0); j < keyCount; j++) {
				auto key = RE::BSScript::Variable();
				if (!a_stream.ReadValue(key)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				if (!isNamespaceValid) {
					continue;
				}

				varSet.insert(std::move(key));
			}
		}

		std::erase_if(_variantSet, [this](const std::pair<const RE::BSFixedString, std::unordered_set<RE::BSScript::Variable>>& a_pair) {
			return !IsNamespaceValid(a_pair.first);
		});

		return true;
	}

	bool SaveScriptVariantSet::Save(SaveStream& a_stream) const
	{
		const auto variableLock = std::shared_lock(_variantSetMutex);

		if (_variantSet.empty()) {
			return true;
		}

		REX::Assert(a_stream.GetMemoryStream().empty());

		if (!a_stream.WriteValue(static_cast<std::uint32_t>(_variantSet.size()))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		for (const auto& [name, varSet] : _variantSet) {
			if (!a_stream.WriteString(name)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!a_stream.WriteValue(static_cast<std::uint32_t>(varSet.size()))) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			for (const auto& key : varSet) {
				if (!a_stream.WriteValue(key)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}
			}
		}

		return true;
	}

	void SaveScriptVariantSet::Revert() noexcept
	{
		const auto variableLock = std::scoped_lock(_variantSetMutex);
		_variantSet.clear();
	}

	bool SaveScriptVariantSet::IsNamespaceValid(const RE::BSFixedString& a_namespace) const
	{
		return ISaveScriptVariantCollection::IsNamespaceValid(a_namespace);
	}

	bool SaveScriptVariantSet::AddValidNamespace(const RE::BSFixedString& a_namespace)
	{
		return ISaveScriptVariantCollection::AddValidNamespace(a_namespace);
	}

	bool SaveScriptVariantSet::Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantSet::Add(a_namespace, a_key);
	}

	bool SaveScriptVariantSet::AddRange(const RE::BSFixedString& a_namespace, std::span<const RE::BSScript::SharedVariant> a_keys)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantSet::AddRange(a_namespace, a_keys);
	}

	bool SaveScriptVariantSet::AddRange(const RE::BSFixedString& a_namespace, std::span<RE::BSScript::Variable> a_keys)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantSet::AddRange(a_namespace, a_keys);
	}
}
