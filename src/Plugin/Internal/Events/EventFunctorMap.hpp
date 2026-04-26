#pragma once

namespace Plugin::Internal::Events
{
	class IEventHandler;
}

namespace Plugin::Internal::Save
{
	class SaveStream;
}

namespace Plugin::Internal::Script
{
	class ScriptFunctionRef;
}

namespace Plugin::Internal::Events
{
	class EventFunctorMap final
	{
	public:
		using GetEventArgsNameType = std::string_view();
		using GetEventParamsNameType = std::string_view();
		using CreateFunctorType = auto(
			const Script::ScriptFunctionRef& a_functionRef,
			const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
			bool a_isPersistent)
			-> REX::NotNull<std::shared_ptr<const IEventHandler>>;
		using LoadFunctorType = bool(const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_eventHandler);
		using SaveFunctorType = bool(Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler);

		class CallbackData final
			: public std::enable_shared_from_this<CallbackData>
		{
		public:
			CallbackData(
				REX::NotNull<std::function<GetEventArgsNameType>> a_getEventArgsNameCallback,
				REX::NotNull<std::function<GetEventParamsNameType>> a_getEventParamsNameCallback,
				REX::NotNull<std::function<CreateFunctorType>> a_createCallback,
				REX::NotNull<std::function<LoadFunctorType>> a_loadCallback,
				REX::NotNull<std::function<SaveFunctorType>> a_saveCallback);

			~CallbackData() noexcept = default;

			CallbackData(const CallbackData&) = default;
			CallbackData(CallbackData&&) noexcept = default;

			CallbackData& operator=(const CallbackData&) = default;
			CallbackData& operator=(CallbackData&&) noexcept = default;

			[[nodiscard]] const auto& GetEventArgsNameCallback() const noexcept { return _getEventArgsNameCallback; }
			[[nodiscard]] const auto& GetEventParamsNameCallback() const noexcept { return _getEventParamsNameCallback; }
			[[nodiscard]] const auto& GetCreateCallback() const noexcept { return _createCallback; }
			[[nodiscard]] const auto& GetLoadCallback() const noexcept { return _loadCallback; }
			[[nodiscard]] const auto& GetSaveCallback() const noexcept { return _saveCallback; }

		private:
			REX::NotNull<std::function<GetEventArgsNameType>> _getEventArgsNameCallback;
			REX::NotNull<std::function<GetEventParamsNameType>> _getEventParamsNameCallback;
			REX::NotNull<std::function<CreateFunctorType>> _createCallback;
			REX::NotNull<std::function<LoadFunctorType>> _loadCallback;
			REX::NotNull<std::function<SaveFunctorType>> _saveCallback;
		};

		EventFunctorMap() = default;
		~EventFunctorMap() noexcept = default;

		EventFunctorMap(const EventFunctorMap&) = delete;
		EventFunctorMap(EventFunctorMap&&) = delete;

		EventFunctorMap& operator=(const EventFunctorMap&) = delete;
		EventFunctorMap& operator=(EventFunctorMap&&) = delete;

		[[nodiscard]] bool Contains(std::string_view a_eventName) const noexcept;

		[[nodiscard]] auto GetValue(std::string_view a_eventName) const -> std::shared_ptr<CallbackData>;
		void SetValue(std::string_view a_eventName, const REX::NotNull<std::shared_ptr<CallbackData>>& a_callbacks);

		bool Add(std::string_view a_eventName, const REX::NotNull<std::shared_ptr<CallbackData>>& a_callbacks);
		bool Remove(std::string_view a_eventName) noexcept;

		void Clear() noexcept;

	private:
		mutable std::shared_mutex _mapMutex;
		std::unordered_map<std::string_view, REX::NotNull<std::shared_ptr<CallbackData>>> _callbackDataMap;
	};

	[[nodiscard]] auto GetEventFunctorMap() -> const REX::NotNull<std::unique_ptr<EventFunctorMap>>&;
}
