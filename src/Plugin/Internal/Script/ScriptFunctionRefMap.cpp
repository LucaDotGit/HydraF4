#include "Plugin/Internal/Script/ScriptFunctionRefMap.hpp"

namespace Plugin::Internal::Script::Impl
{
	bool IsInScript(
		const RE::BSFixedString& a_scriptName,
		const ScriptFunctionRef& a_functionRef) noexcept
	{
		return a_functionRef.GetScriptName() == a_scriptName;
	}
}
