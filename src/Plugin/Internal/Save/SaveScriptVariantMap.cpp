#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveScriptVariantMap::SaveScriptVariantMap(std::uint32_t a_type, std::uint32_t a_version)
		: ISaveScriptVariantCollection(a_type, a_version)
	{
	}

	SaveScriptVariantMap::~SaveScriptVariantMap() noexcept = default;

	bool SaveScriptVariantMap::Load(const SaveStream& a_stream)
	{
		const auto variableLock = std::scoped_lock(_variantMapMutex);

		auto nameCount = static_cast<std::uint32_t>(0);
		if (!a_stream.ReadValue(nameCount)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (nameCount == 0) {
			return true;
		}

		_variantMap.reserve(nameCount);

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

			if (keyCount <= 0) {
				continue;
			}

			const auto isNamespaceValid = IsNamespaceValid(name);

			auto& varMap = _variantMap[name];
			varMap.reserve(keyCount);

			for (auto j = static_cast<std::uint32_t>(0); j < keyCount; j++) {
				auto key = RE::BSScript::Variable();
				if (!a_stream.ReadValue(key)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				auto value = RE::BSScript::Variable();
				if (!a_stream.ReadValue(value)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				if (!isNamespaceValid) {
					continue;
				}

				varMap.emplace(std::move(key), std::move(value));
			}
		}

		std::erase_if(_variantMap, [this](const std::pair<const RE::BSFixedString, std::unordered_map<RE::BSScript::Variable, RE::BSScript::Variable>>& a_pair) {
			return !IsNamespaceValid(a_pair.first);
		});

		return true;
	}

	bool SaveScriptVariantMap::Save(SaveStream& a_stream) const
	{
		const auto variableLock = std::shared_lock(_variantMapMutex);

		if (_variantMap.empty()) {
			return true;
		}

		REX::Assert(a_stream.GetMemoryStream().empty());

		if (!a_stream.WriteValue(static_cast<std::uint32_t>(_variantMap.size()))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		for (const auto& [name, varMap] : _variantMap) {
			if (!a_stream.WriteString(name)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!a_stream.WriteValue(static_cast<std::uint32_t>(varMap.size()))) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			for (const auto& [key, value] : varMap) {
				if (!a_stream.WriteValue(key)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				if (!a_stream.WriteValue(value)) [[unlikely]] {
					REX::Assert(false);
					return false;
				}
			}
		}

		return true;
	}

	void SaveScriptVariantMap::Revert() noexcept
	{
		const auto variableLock = std::scoped_lock(_variantMapMutex);
		_variantMap.clear();
	}

	bool SaveScriptVariantMap::IsNamespaceValid(const RE::BSFixedString& a_namespace) const
	{
		return ISaveScriptVariantCollection::IsNamespaceValid(a_namespace);
	}

	bool SaveScriptVariantMap::AddValidNamespace(const RE::BSFixedString& a_namespace)
	{
		return ISaveScriptVariantCollection::AddValidNamespace(a_namespace);
	}

	bool SaveScriptVariantMap::SetPairs(const RE::BSFixedString& a_namespace, std::span<const std::optional<Structs::VarPairs::VarPairData>> a_pairs)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::SetPairs(a_namespace, a_pairs);
	}

	bool SaveScriptVariantMap::SetValue(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::SetValue(a_namespace, a_key, a_value);
	}

	bool SaveScriptVariantMap::SetRange(const RE::BSFixedString& a_namespace,
		std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::SetRange(a_namespace, a_pairs);
	}

	bool SaveScriptVariantMap::SetRange(const RE::BSFixedString& a_namespace,
		std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::SetRange(a_namespace, a_pairs);
	}

	bool SaveScriptVariantMap::Update(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_newValue, RE::BSScript::SharedVariant a_compareValue)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::Update(a_namespace, a_key, a_newValue, a_compareValue);
	}

	bool SaveScriptVariantMap::Add(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::Add(a_namespace, a_key, a_value);
	}

	bool SaveScriptVariantMap::AddRange(const RE::BSFixedString& a_namespace,
		std::span<const std::pair<RE::BSScript::SharedVariant, RE::BSScript::SharedVariant>> a_pairs)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::AddRange(a_namespace, a_pairs);
	}

	bool SaveScriptVariantMap::AddRange(const RE::BSFixedString& a_namespace,
		std::span<std::pair<RE::BSScript::Variable, RE::BSScript::Variable>> a_pairs)
	{
		if (!IsNamespaceValid(a_namespace)) {
			return false;
		}

		return ScriptVariantMap::AddRange(a_namespace, a_pairs);
	}

	auto SaveScriptVariantMap::AddOrUpdate(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_addValue, RE::BSScript::SharedVariant a_updateValue) -> RE::BSScript::UniqueVariant
	{
		if (!IsNamespaceValid(a_namespace)) {
			return nullptr;
		}

		return ScriptVariantMap::AddOrUpdate(a_namespace, a_key, a_addValue, a_updateValue);
	}

	auto SaveScriptVariantMap::GetOrAdd(const RE::BSFixedString& a_namespace, RE::BSScript::SharedVariant a_key,
		RE::BSScript::SharedVariant a_value)
		-> RE::BSScript::UniqueVariant
	{
		if (!IsNamespaceValid(a_namespace)) {
			return nullptr;
		}

		return ScriptVariantMap::GetOrAdd(a_namespace, a_key, a_value);
	}
}