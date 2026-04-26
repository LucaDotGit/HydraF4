#pragma once

#include "Plugin/Internal/Script/IScriptValueFilter.hpp"

namespace Plugin::Internal::Script
{
	class ScriptVariantFilter final
		: public IScriptValueFilter<const RE::TESForm*>
	{
	public:
		class VariantFilter final
		{
		public:
			VariantFilter() = default;
			~VariantFilter() noexcept = default;

			VariantFilter(const VariantFilter&) = default;
			VariantFilter(VariantFilter&&) noexcept = default;

			VariantFilter& operator=(const VariantFilter&) = default;
			VariantFilter& operator=(VariantFilter&&) noexcept = default;

			[[nodiscard]] bool empty() const noexcept;

			void InsertFilter(RE::BSScript::SharedVariant a_value);
			void InsertFilter(std::span<const RE::BSScript::SharedVariant> a_values);

			std::unordered_set<RE::FormType> formTypes;
			std::unordered_set<REX::NotNull<const RE::TESFile*>> plugins;
			std::unordered_set<RE::BSScript::ObjectHandle> objectHandles;
		};

		ScriptVariantFilter() = default;
		~ScriptVariantFilter() noexcept override = default;

		ScriptVariantFilter(
			std::span<const RE::BSScript::SharedVariant> a_includedValues,
			std::span<const RE::BSScript::SharedVariant> a_excludedValues);

		template <REX::StaticString ScriptName, REX::StaticString StructName>
		ScriptVariantFilter(
			const RE::BSScript::StructWrapper<ScriptName, StructName>& a_struct,
			std::string_view a_includedValuesKey,
			std::string_view a_excludedValuesKey);

		ScriptVariantFilter(const ScriptVariantFilter&) = default;
		ScriptVariantFilter(ScriptVariantFilter&&) noexcept = default;

		ScriptVariantFilter& operator=(const ScriptVariantFilter&) = default;
		ScriptVariantFilter& operator=(ScriptVariantFilter&&) noexcept = default;

		[[nodiscard]] const VariantFilter& GetIncludedValues() const noexcept { return _includedValues; }
		[[nodiscard]] const VariantFilter& GetExcludedValues() const noexcept { return _excludedValues; }

		[[nodiscard]] bool IsMatch(const RE::TESForm* const& a_form) const override;

	protected:
		VariantFilter _includedValues;
		VariantFilter _excludedValues;
	};
}

#include "Plugin/Internal/Script/ScriptVariantFilter.inl"
