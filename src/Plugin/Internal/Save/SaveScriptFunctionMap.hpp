#pragma once

#include "Plugin/Internal/Save/ISaveObject.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Events
{
	class EventSystem;
}

namespace Plugin::Internal::Save
{
	class SaveScriptFunctionMap final
		: public ISaveObject
	{
	public:
		class EventInfo final
		{
		public:
			RE::BSFixedString eventName;
			Script::ScriptFunctionRef functionRef;
			std::unordered_map<RE::BSFixedString, json_t> eventArgs;
		};

		SaveScriptFunctionMap(std::uint32_t a_type, std::uint32_t a_version,
			const REX::NotNull<std::shared_ptr<Events::EventSystem>>& a_eventSystem);

		~SaveScriptFunctionMap() noexcept override;

		SaveScriptFunctionMap(const SaveScriptFunctionMap&) = delete;
		SaveScriptFunctionMap(SaveScriptFunctionMap&&) = delete;

		SaveScriptFunctionMap& operator=(const SaveScriptFunctionMap&) = delete;
		SaveScriptFunctionMap& operator=(SaveScriptFunctionMap&&) = delete;

		bool Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool) override;
		bool Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const override;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

		[[nodiscard]] bool IsEmpty() const noexcept;
		[[nodiscard]] std::uint32_t GetSize() const noexcept;

		void AddValue(REX::NotNull<std::shared_ptr<EventInfo>> a_eventInfo);

		void Clear() noexcept;

	private:
		static auto CreateEventArgsStruct(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const RE::BSFixedString& a_structName,
			const std::unordered_map<RE::BSFixedString, json_t>& a_eventArgs)
			-> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>;

		REX::NotNull<std::shared_ptr<Events::EventSystem>> _eventSystem;
		mutable std::shared_mutex _mapMutex;
		std::unordered_multimap<RE::BSFixedString, REX::NotNull<std::shared_ptr<EventInfo>>> _eventMap;
	};
}
