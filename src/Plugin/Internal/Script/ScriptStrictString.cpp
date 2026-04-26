#include "Plugin/Internal/Script/ScriptStrictString.hpp"

#include "Plugin/Internal/Script/ScriptStrictStringPool.hpp"

namespace Plugin::Internal::Script
{
	ScriptStrictString::ScriptStrictString(key_type a_key, value_type a_value)
		: _key(std::move(a_key)),
		  _value(std::move(a_value))
	{
	}
}

namespace RE::BSScript
{
	auto variable_packer<::Plugin::Internal::Script::ScriptStrictString>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<RE::BSFixedStringCS>();
	}

	void variable_packer<::Plugin::Internal::Script::ScriptStrictString>::PackVariable(Variable& a_var, const ::Plugin::Internal::Script::ScriptStrictString& a_val)
	{
		RE::BSScript::PackVariable(a_var, a_val.GetValue());
	}

	auto variable_packer<::Plugin::Internal::Script::ScriptStrictString>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Script::ScriptStrictString
	{
		auto key = RE::BSScript::UnpackVariable<RE::BSFixedString>(a_var);
		auto value = ::Plugin::Internal::Script::GetScriptStrictStringPool()->GetValue(key);

		if (!value.has_value()) {
			value = key;
		}

		return { key, std::move(value).value() };
	}
}
