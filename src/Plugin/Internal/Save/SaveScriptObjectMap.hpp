#pragma once

#include "Plugin/Internal/Save/ISaveObject.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptObjectMap final
		: public ISaveObject
	{
	public:
		class ObjectInfo final
		{
		public:
			RE::BSFixedString scriptName;
			RE::BSScript::ObjectHandle objectHandle{ RE::GameScript::HandlePolicy::EMPTY_HANDLE };
			std::unordered_map<RE::BSFixedString, json_t> properties;
		};

		SaveScriptObjectMap(std::uint32_t a_type, std::uint32_t a_version);
		~SaveScriptObjectMap() noexcept override;

		SaveScriptObjectMap(const SaveScriptObjectMap&) = delete;
		SaveScriptObjectMap(SaveScriptObjectMap&&) = delete;

		SaveScriptObjectMap& operator=(const SaveScriptObjectMap&) = delete;
		SaveScriptObjectMap& operator=(SaveScriptObjectMap&&) = delete;

		bool Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool) override;
		bool Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const override;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

		[[nodiscard]] bool IsEmpty() const noexcept;
		[[nodiscard]] std::uint32_t GetSize() const noexcept;

		void AddValue(REX::NotNull<std::shared_ptr<ObjectInfo>> a_objectInfo);

		void Clear() noexcept;

	private:
		static auto CreateInitProperties(
			const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>>& a_objectTypeInfo,
			const std::unordered_map<RE::BSFixedString, json_t>& a_templateProperties) -> RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable>;

		static bool AutoFillProperties(
			const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>>& a_objectTypeInfo,
			const REX::NotNull<std::function<RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable>()>>& a_propertiesFactory,
			const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::Object>>& a_scriptObject);

		mutable std::shared_mutex _mapMutex;
		std::unordered_multimap<RE::BSFixedString, REX::NotNull<std::shared_ptr<ObjectInfo>>> _scriptObjectMap;
	};
}
