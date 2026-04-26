#pragma once

#include "Plugin/Internal/Script/ScriptValueFilter.hpp"

namespace Plugin::Internal::Script
{
	template <class T>
	ScriptValueFilter<T>::ScriptValueFilter(
		std::span<const T> a_includedStrings,
		std::span<const T> a_excludedStrings)
		: _includedValues{ a_includedStrings.begin(), a_includedStrings.end() },
		  _excludedValues{ a_excludedStrings.begin(), a_excludedStrings.end() }
	{
	}

	template <class T>
	template <REX::StaticString ScriptName, REX::StaticString StructName>
	ScriptValueFilter<T>::ScriptValueFilter(
		const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
		std::string_view a_includedValuesKey,
		std::string_view a_excludedValuesKey)
		: ScriptValueFilter(
			  a_struct.template FindOrDefault<std::vector<T>>(a_includedValuesKey),
			  a_struct.template FindOrDefault<std::vector<T>>(a_excludedValuesKey))
	{
	}

	template <class T>
	bool ScriptValueFilter<T>::IsMatch(const T& a_value) const
	{
		return (_includedValues.empty() || _includedValues.contains(a_value)) &&
			   (_excludedValues.empty() || !_excludedValues.contains(a_value));
	}
}
