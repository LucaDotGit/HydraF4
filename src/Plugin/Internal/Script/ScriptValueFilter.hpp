#pragma once

#include "Plugin/Internal/Script/IScriptValueFilter.hpp"

namespace Plugin::Internal::Script
{
	template <class T>
	class ScriptValueFilter final
		: public IScriptValueFilter<T>
	{
	public:
		using value_type = T;

		ScriptValueFilter() = default;

		~ScriptValueFilter() //
			noexcept(std::is_nothrow_destructible_v<T>) override = default;

		ScriptValueFilter(
			std::span<const T> a_includedStrings,
			std::span<const T> a_excludedStrings);

		template <REX::StaticString ScriptName, REX::StaticString StructName>
		ScriptValueFilter(
			const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
			std::string_view a_includedValuesKey,
			std::string_view a_excludedValuesKey);

		ScriptValueFilter(const ScriptValueFilter&) = default;

		ScriptValueFilter(ScriptValueFilter&&) //
			noexcept(std::is_nothrow_move_constructible_v<T>) = default;

		ScriptValueFilter& operator=(const ScriptValueFilter&) = default;

		ScriptValueFilter& operator=(ScriptValueFilter&&) //
			noexcept(std::is_nothrow_move_assignable_v<T>) = default;

		[[nodiscard]] const std::unordered_set<T>& GetIncludedValues() const noexcept { return _includedValues; }
		[[nodiscard]] const std::unordered_set<T>& GetExcludedValues() const noexcept { return _excludedValues; }

		[[nodiscard]] bool IsMatch(const T& a_value) const override;

	protected:
		std::unordered_set<T> _includedValues;
		std::unordered_set<T> _excludedValues;
	};
}

#include "Plugin/Internal/Script/ScriptValueFilter.inl"
