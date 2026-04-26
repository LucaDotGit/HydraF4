#include "Plugin/Internal/Events/IEventHandler.hpp"

namespace Plugin::Internal::Events
{
	IEventHandler::IEventHandler(Script::ScriptFunctionRef a_functionRef)
		: _functionRef(std::move(a_functionRef))
	{
	}

	IEventHandler::IEventHandler(Script::ScriptFunctionRef a_functionRef, bool a_isPersistent)
		: _functionRef(std::move(a_functionRef)),
		  _isPersistent(a_isPersistent)
	{
	}

	IEventHandler::IEventHandler(Script::ScriptFunctionRef a_functionRef, const RE::BSTSmartPointer<RE::BSScript::Struct>& a_args, bool a_isPersistent)
		: _functionRef(std::move(a_functionRef)),
		  _args(a_isPersistent ? a_args : nullptr),
		  _isPersistent(a_isPersistent)
	{
	}
}
