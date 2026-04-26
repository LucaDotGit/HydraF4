#pragma once

#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Script
{
	template <class... Args>
	bool ScriptFunctionRef::InvokeFunction(Args&&... a_args) const
		requires((RE::BSScript::ValidateParameter<std::remove_reference_t<Args>>() && ...))
	{
		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return InvokeFunctionWithCallback(STACK_CALLBACK, std::forward<Args>(a_args)...);
	}

	template <class... Args>
	bool ScriptFunctionRef::InvokeFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor, Args&&... a_args) const
		requires((RE::BSScript::ValidateParameter<std::remove_reference_t<Args>>() && ...))
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			return scriptVM->InvokeMemberFunction(GetObject(), _functionName, a_callbackFunctor, std::forward<Args>(a_args)...);
		}

		return scriptVM->InvokeStaticFunction(_scriptName, _functionName, a_callbackFunctor, std::forward<Args>(a_args)...);
	}
}
