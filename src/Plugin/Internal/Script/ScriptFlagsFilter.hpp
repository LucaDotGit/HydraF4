#pragma once

#include "Plugin/Internal/Script/IScriptValueFilter.hpp"

namespace Plugin::Internal::Script
{
	template <class T>
		requires(std::is_enum_v<T>)
	class ScriptFlagsFilter final
		: public IScriptValueFilter<T>
	{
	public:
		using value_type = T;

		ScriptFlagsFilter() = default;
		~ScriptFlagsFilter() noexcept override = default;

		ScriptFlagsFilter(T a_includedFlags, T a_excludedFlags);
		ScriptFlagsFilter(T a_includedFlags, T a_excludedFlags, T a_defaultFlag);

		template <REX::StaticString ScriptName, REX::StaticString StructName>
		ScriptFlagsFilter(
			const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
			std::string_view a_includedFlagsKey,
			std::string_view a_excludedFlagsKey);

		template <REX::StaticString ScriptName, REX::StaticString StructName>
		ScriptFlagsFilter(
			const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
			std::string_view a_includedFlagsKey,
			std::string_view a_excludedFlagsKey, T a_defaultFlag);

		ScriptFlagsFilter(const ScriptFlagsFilter&) = default;
		ScriptFlagsFilter(ScriptFlagsFilter&&) noexcept = default;

		ScriptFlagsFilter& operator=(const ScriptFlagsFilter&) = default;
		ScriptFlagsFilter& operator=(ScriptFlagsFilter&&) noexcept = default;

		[[nodiscard]] T GetIncludedFlags() const noexcept { return _includedFlags; }
		[[nodiscard]] T GetExcludedFlags() const noexcept { return _excludedFlags; }
		[[nodiscard]] T GetDefaultFlag() const noexcept { return _defaultFlag; }

		[[nodiscard]] bool IsMatch(const T& a_value) const override;

	protected:
		T _includedFlags;
		T _excludedFlags;
		T _defaultFlag;
	};
}

#include "Plugin/Internal/Script/ScriptFlagsFilter.inl"
