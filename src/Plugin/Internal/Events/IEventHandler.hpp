#pragma once

#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Events
{
	class IEventHandler;

	template <class T>
	concept IEventHandlerConstraint =
		std::derived_from<T, IEventHandler> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;
}

namespace Plugin::Internal::Events
{
	class IEventHandler
		: public std::enable_shared_from_this<IEventHandler>
	{
	public:
		IEventHandler(Script::ScriptFunctionRef a_functionRef);
		IEventHandler(Script::ScriptFunctionRef a_functionRef, bool a_isPersistent);
		IEventHandler(Script::ScriptFunctionRef a_functionRef, const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args, bool a_isPersistent);

		virtual ~IEventHandler() noexcept = default;

		IEventHandler(const IEventHandler&) = delete;
		IEventHandler(IEventHandler&&) = delete;

		IEventHandler& operator=(const IEventHandler&) = delete;
		IEventHandler& operator=(IEventHandler&&) = delete;

		[[nodiscard]] virtual std::string_view GetName() const noexcept = 0;
		[[nodiscard]] virtual std::string_view GetEventArgsName() const noexcept = 0;
		[[nodiscard]] virtual std::string_view GetEventParamsName() const noexcept = 0;

		[[nodiscard]] virtual auto GetFunctionRef() const noexcept -> const Script::ScriptFunctionRef& { return _functionRef; }
		[[nodiscard]] virtual auto GetArgs() const noexcept -> RE::BSTSmartPointer<RE::BSScript::Struct> { return _args; }
		[[nodiscard]] virtual auto GetIsPersistent() const noexcept -> bool { return _isPersistent; }

	protected:
		Script::ScriptFunctionRef _functionRef;
		RE::BSTSmartPointer<RE::BSScript::Struct> _args{ nullptr };
		bool _isPersistent{ false };
	};
}
