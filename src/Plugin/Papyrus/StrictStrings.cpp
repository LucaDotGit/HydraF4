#include "Plugin/Papyrus/StrictStrings.hpp"

#include "Plugin/Internal/Script/ScriptStrictStringPool.hpp"

namespace Plugin::Papyrus::StrictStrings
{
	static bool Contains(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		return ::Plugin::Internal::Script::GetScriptStrictStringPool()->Contains(a_key);
	}

	static RE::BSFixedString GetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		RE::BSFixedString a_default)
	{
		auto value = ::Plugin::Internal::Script::GetScriptStrictStringPool()->GetValue(a_key);
		if (!value.has_value()) {
			return a_default;
		}

		return RE::BSFixedString(std::move(value).value());
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetValue);
	}
}
