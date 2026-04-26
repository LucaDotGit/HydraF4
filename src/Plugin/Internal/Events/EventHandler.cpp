#include "Plugin/Internal/Events/EventHandler.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Events::Impl
{
	bool Load(const Save::SaveStream& a_stream, std::shared_ptr<const IEventHandler>& a_eventHandler,
		const REX::NotNull<std::function<auto(
			const Script::ScriptFunctionRef& a_functionRef,
			const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args,
			bool a_isPersistent)
				->REX::NotNull<std::shared_ptr<const IEventHandler>>>>& a_eventHandlerFactory)
	{
		auto isPersistent = false;
		if (!a_stream.ReadValue(isPersistent)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto functionRef = Script::ScriptFunctionRef::LoadStream(a_stream);
		if (!functionRef.has_value() && functionRef.error() == Script::ScriptFunctionRef::LoadError::Failure) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto args = RE::BSTSmartPointer<RE::BSScript::Struct>();
		if (!a_stream.ReadStruct(args)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		REX::Assert(isPersistent);

		if (!functionRef.has_value()) {
			a_eventHandler.reset();
			return true;
		}

		auto createdEventHandler = std::invoke(*a_eventHandlerFactory, functionRef.value(), args, isPersistent);
		a_eventHandler = *std::move(createdEventHandler);
		return true;
	}

	bool Save(Save::SaveStream& a_stream, const REX::NotNull<std::shared_ptr<const IEventHandler>>& a_eventHandler)
	{
		const auto isPersistent = a_eventHandler->GetIsPersistent();
		if (!isPersistent) {
			return true;
		}

		if (!a_stream.WriteValue(isPersistent)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto& functionRef = a_eventHandler->GetFunctionRef();
		if (!functionRef.SaveStream(a_stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto& args = a_eventHandler->GetArgs();
		if (!a_stream.WriteStruct(args)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}
}
