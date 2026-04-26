#pragma once

#include "Plugin/Internal/Events/EventFunctorMap.hpp"
#include "Plugin/Internal/Events/IEventHandler.hpp"

// IWYU pragma: begin_exports
#include "Plugin/Internal/Events/EventStructs.hpp"
#include "Plugin/Internal/Script/ScriptFlagsFilter.hpp"
#include "Plugin/Internal/Script/ScriptValueFilter.hpp"
#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"
// IWYU pragma: end_exports

namespace Plugin::Internal::Events
{
	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	class EventHandler;

	template <class T>
	concept EventHandlerConstraint =
		std::derived_from<T,
			EventHandler<
				typename T::EventType,
				typename T::EventArgs,
				typename T::EventParams,
				T::EVENT_NAME>>;
}

namespace Plugin::Internal::Save
{
	class SaveStream;
}

namespace Plugin::Internal::Events
{
	template <
		class EventT,
		RE::BSScript::Impl::vm_struct_wrapper TEventArgs,
		RE::BSScript::Impl::vm_struct_wrapper TEventParams,
		REX::StaticString EventName>
	class EventHandler
		: public IEventHandler
	{
	public:
		using EventType = EventT;
		using EventArgs = TEventArgs;
		using EventParams = TEventParams;

		inline static constexpr auto EVENT_NAME = EventName;

		EventHandler(const Script::ScriptFunctionRef& a_functionRef, const EventArgs& a_args, bool a_isPersistent);
		~EventHandler() noexcept override = default;

		EventHandler(const EventHandler&) = delete;
		EventHandler(EventHandler&&) = delete;

		EventHandler& operator=(const EventHandler&) = delete;
		EventHandler& operator=(EventHandler&&) = delete;

		[[nodiscard]] std::string_view GetName() const noexcept final { return EVENT_NAME; }
		[[nodiscard]] std::string_view GetEventArgsName() const noexcept final { return EventArgs::TYPE_NAME; }
		[[nodiscard]] std::string_view GetEventParamsName() const noexcept final { return EventParams::TYPE_NAME; }

		[[nodiscard]] static auto Create(
			const Script::ScriptFunctionRef& a_functionRef,
			const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
			bool a_isPersistent)
			-> REX::NotNull<std::shared_ptr<const IEventHandler>>;
		[[nodiscard]] static bool Load(const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_outResult);
		[[nodiscard]] static bool Save(Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler);

	private:
		inline static const auto STATIC_CTOR = []() {
			auto getEventArgsName = REX::NotNull(std::function(
				[]() noexcept {
					return static_cast<std::string_view>(EventArgs::TYPE_NAME);
				}));

			auto getEventParamsName = REX::NotNull(std::function(
				[]() noexcept {
					return static_cast<std::string_view>(EventParams::TYPE_NAME);
				}));

			auto createFunctor = REX::NotNull(std::function(
				[](const Script::ScriptFunctionRef& a_functionRef, const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args, bool a_isPersistent) {
					return EventHandler::Create(a_functionRef, a_args, a_isPersistent);
				}));

			auto loadFunctor = REX::NotNull(std::function(
				[](const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_outResult) {
					auto eventHandler = std::shared_ptr<const IEventHandler>();
					if (!EventHandler::Load(a_stream, eventHandler)) [[unlikely]] {
						REX::Assert(false);
						return false;
					}

					a_outResult = std::move(eventHandler);
					return true;
				}));

			auto saveFunctor = REX::NotNull(std::function(
				[](Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler) {
					if (!EventHandler::Save(a_stream, a_eventHandler)) [[unlikely]] {
						REX::Assert(false);
						return false;
					}

					return true;
				}));

			const auto callbackData = REX::NotNull(std::make_shared<EventFunctorMap::CallbackData>(
				std::move(getEventArgsName),
				std::move(getEventParamsName),
				std::move(createFunctor),
				std::move(loadFunctor),
				std::move(saveFunctor)));

			if (!GetEventFunctorMap()->Add(EVENT_NAME, callbackData)) [[unlikely]] {
				REX::Fail(R"(Failed to register event handler with name "{}".)",
					static_cast<std::string_view>(EVENT_NAME));
				return false;
			}

			return true;
		}();
	};
}

namespace Plugin::Internal::Events::Impl
{
	template <class T>
	concept EventFunctorConstraint =
		IEventHandlerConstraint<T> &&
		std::is_constructible_v<T, Script::ScriptFunctionRef, typename T::EventArgs, bool>;

	bool Load(const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_eventHandler,
		const REX::NotNull<std::function<auto(
			const Script::ScriptFunctionRef& a_functionRef,
			const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
			bool a_isPersistent)
				->REX::NotNull<std::shared_ptr<const IEventHandler>>>>& a_eventHandlerFactory);
	bool Save(Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler);
}

namespace Plugin::Internal::Events
{
	template <class>
	struct event_traits
	{
		using ValidateCallback = std::false_type;
	};

	template <IEventHandlerConstraint T>
	struct event_traits<T>
	{
		using ValidateCallback = std::true_type;
	};

	template <Impl::EventFunctorConstraint T>
	class event_functor final
	{
	public:
		event_functor() = delete;

		static auto Create(
			const Script::ScriptFunctionRef& a_functionRef,
			const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
			bool a_isPersistent)
			-> REX::NotNull<std::shared_ptr<const IEventHandler>>;
		static bool Load(const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_eventHandler);
		static bool Save(Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler);
	};
}

#include "Plugin/Internal/Events/EventHandler.inl"
