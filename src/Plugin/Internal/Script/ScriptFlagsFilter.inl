#pragma once

#include "Plugin/Internal/Script/ScriptFlagsFilter.hpp"

namespace Plugin::Internal::Script
{
	template <class T>
		requires(std::is_enum_v<T>)
	ScriptFlagsFilter<T>::ScriptFlagsFilter(T a_includedFlags, T a_excludedFlags)
		: _includedFlags(a_includedFlags),
		  _excludedFlags(a_excludedFlags),
		  _defaultFlag(static_cast<T>(0))
	{
	}

	template <class T>
		requires(std::is_enum_v<T>)
	ScriptFlagsFilter<T>::ScriptFlagsFilter(T a_includedFlags, T a_excludedFlags, T a_defaultFlag)
		: _includedFlags(a_includedFlags),
		  _excludedFlags(a_excludedFlags),
		  _defaultFlag(a_defaultFlag)
	{
	}

	template <class T>
		requires(std::is_enum_v<T>)
	template <REX::StaticString ScriptName, REX::StaticString StructName>
	ScriptFlagsFilter<T>::ScriptFlagsFilter(
		const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
		std::string_view a_includedFlagsKey,
		std::string_view a_excludedFlagsKey)
		: _includedFlags(a_struct.template FindOrDefault<T>(a_includedFlagsKey)),
		  _excludedFlags(a_struct.template FindOrDefault<T>(a_excludedFlagsKey)),
		  _defaultFlag(static_cast<T>(0))
	{
	}

	template <class T>
		requires(std::is_enum_v<T>)
	template <REX::StaticString ScriptName, REX::StaticString StructName>
	ScriptFlagsFilter<T>::ScriptFlagsFilter(
		const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
		std::string_view a_includedFlagsKey,
		std::string_view a_excludedFlagsKey, T a_defaultFlag)
		: _includedFlags(a_struct.template FindOrDefault<T>(a_includedFlagsKey)),
		  _excludedFlags(a_struct.template FindOrDefault<T>(a_excludedFlagsKey)),
		  _defaultFlag(a_defaultFlag)
	{
	}

	template <class T>
		requires(std::is_enum_v<T>)
	bool ScriptFlagsFilter<T>::IsMatch(const T& a_value) const
	{
		if (_includedFlags == _defaultFlag && _excludedFlags == _defaultFlag) {
			return true;
		}

		const auto value = REX::EnumSet(a_value);
		return value.any(_includedFlags) && value.none(_excludedFlags);
	}
}
