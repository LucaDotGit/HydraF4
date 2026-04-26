#pragma once

#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"

namespace Plugin::Internal::Script
{
	template <REX::StaticString ScriptName, REX::StaticString StructName>
	ScriptVariantFilter::ScriptVariantFilter(
		const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
		std::string_view a_includedValuesKey,
		std::string_view a_excludedValuesKey)
		: ScriptVariantFilter(
			  a_struct.template FindOrDefault<std::vector<RE::BSScript::SharedVariant>>(a_includedValuesKey),
			  a_struct.template FindOrDefault<std::vector<RE::BSScript::SharedVariant>>(a_excludedValuesKey))
	{
	}
}
