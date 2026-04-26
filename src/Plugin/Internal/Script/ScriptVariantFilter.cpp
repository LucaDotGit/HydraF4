#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"

namespace Plugin::Internal::Script::Impl
{
	template <class T>
	[[nodiscard]] static bool FilterByValue(
		const T& a_value,
		const std::unordered_set<T>& a_includedValues,
		const std::unordered_set<T>& a_excludedValues)
	{
		return (a_includedValues.empty() || a_includedValues.contains(a_value)) &&
			   (a_excludedValues.empty() || !a_excludedValues.contains(a_value));
	}

	[[nodiscard]] static bool FilterByFormTypes(
		const RE::TESForm* a_form,
		const std::unordered_set<RE::FormType>& a_includedFormTypes,
		const std::unordered_set<RE::FormType>& a_excludedFormTypes)
	{
		if (a_includedFormTypes.empty() && a_excludedFormTypes.empty()) {
			return true;
		}

		if (!a_form) {
			return false;
		}

		if (FilterByValue(a_form->GetFormType(), a_includedFormTypes, a_excludedFormTypes)) {
			return true;
		}

		const auto* ref = RE::DynamicCast<const RE::TESObjectREFR*>(a_form);
		if (!ref) {
			return false;
		}

		const auto* refBase = ref->GetBaseObject();
		if (!refBase) {
			return false;
		}

		return FilterByValue(refBase->GetFormType(), a_includedFormTypes, a_excludedFormTypes);
	}

	[[nodiscard]] static bool FilterByPlugins(
		const RE::TESForm* a_form,
		const std::unordered_set<REX::NotNull<const RE::TESFile*>>& a_includedPlugins,
		const std::unordered_set<REX::NotNull<const RE::TESFile*>>& a_excludedPlugins)
	{
		if (a_includedPlugins.empty() && a_excludedPlugins.empty()) {
			return true;
		}

		if (!a_form) {
			return false;
		}

		const auto filterPredicate = [&a_includedPlugins, &a_excludedPlugins](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			const auto* pluginArray = a_form->sourceFiles.array;
			if (pluginArray && !pluginArray->empty()) {
				return std::ranges::any_of(*pluginArray, [&a_includedPlugins, &a_excludedPlugins](REX::NotNull<const RE::TESFile*> a_plugin) {
					return FilterByValue(a_plugin, a_includedPlugins, a_excludedPlugins);
				});
			}

			if (!a_form->IsPlayer() && !a_form->IsPlayerRef()) {
				return false;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Fail("Failed to get the game's data handler."sv);
			}

			const auto* mainPlugin = dataHandler->FindLoadedFileByIndex(0);
			if (!mainPlugin) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			return FilterByValue(REX::NotNull(mainPlugin), a_includedPlugins, a_excludedPlugins);
		};

		if (filterPredicate(a_form)) {
			return true;
		}

		const auto* ref = RE::DynamicCast<const RE::TESObjectREFR*>(a_form);
		if (!ref) {
			return false;
		}

		const auto* refBase = ref->GetBaseObject();
		if (!refBase) {
			return false;
		}

		return filterPredicate(refBase);
	}

	[[nodiscard]] static bool FilterByForm(
		const RE::TESForm* a_form,
		const RE::TESForm* a_formFilter)
	{
		if (!a_form || !a_formFilter) {
			return false;
		}

		if (a_form == a_formFilter) {
			return true;
		}

		switch (a_formFilter->GetFormType()) {
			case RE::BGSKeyword::FORM_TYPE: {
				const auto* keyword = RE::DynamicCast<const RE::BGSKeyword*>(a_formFilter);
				if (!keyword) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				return RE::TESForm::ContainsKeyword(a_form, keyword).value_or(false);
			}
			case RE::TESFaction::FORM_TYPE: {
				const auto* faction = RE::DynamicCast<const RE::TESFaction*>(a_formFilter);
				if (!faction) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				const auto* actor = RE::DynamicCast<const RE::Actor*>(a_form);
				if (actor) {
					return actor->IsInFaction(faction);
				}

				const auto* actorBase = RE::DynamicCast<const RE::TESNPC*>(a_form);
				if (actorBase) {
					return actorBase->IsInFaction(faction);
				}

				break;
			}
			case RE::TESRace::FORM_TYPE: {
				const auto* race = RE::DynamicCast<const RE::TESRace*>(a_formFilter);
				if (!race) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				const auto* actor = RE::DynamicCast<const RE::Actor*>(a_form);
				if (actor) {
					return actor->race == race;
				}

				const auto* raceHolder = RE::DynamicCast<const RE::TESRaceForm*>(a_form);
				if (raceHolder) {
					return raceHolder->formRace == race;
				}

				break;
			}
			case RE::TESNPC::FORM_TYPE: {
				const auto* actorBase = RE::DynamicCast<const RE::TESNPC*>(a_formFilter);
				if (!actorBase) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				const auto* actor = RE::DynamicCast<const RE::Actor*>(a_form);
				if (!actor) {
					break;
				}

				return actor->GetTemplateBase() == actorBase;
			}
			case RE::BGSListForm::FORM_TYPE: {
				using ForEachResultType = RE::BSContainer::ForEachResult;

				const auto* formList = RE::DynamicCast<const RE::BGSListForm*>(a_formFilter);
				if (!formList) [[unlikely]] {
					REX::Assert(false);
					return false;
				}

				return formList->ForEachForm([a_form](const RE::TESForm* a_element) {
					return FilterByForm(a_form, a_element) ? ForEachResultType::kStop : ForEachResultType::kContinue;
				}) == ForEachResultType::kStop;
			}
			default: {
				const auto* ref = RE::DynamicCast<const RE::TESObjectREFR*>(a_form);
				if (!ref) {
					break;
				}

				const auto* refBase = ref->GetBaseObject();
				if (!refBase) {
					break;
				}

				return refBase == a_formFilter;
			}
		}

		return false;
	}

	[[nodiscard]] static bool FilterByObjectHandle(
		const RE::TESForm* a_form,
		RE::BSScript::ObjectHandle a_objectHandleFilter)
	{
		if (!a_form) {
			return false;
		}

		if (!RE::GameScript::HandlePolicy::IsFormHandle(a_objectHandleFilter)) {
			return false;
		}

		const auto formId = RE::GameScript::HandlePolicy::GetFormIDByHandle(a_objectHandleFilter);
		if (formId == RE::TESDataHandler::EMPTY_FORM_ID) {
			return false;
		}

		const auto* form = RE::TESForm::FindFormByNumericID(formId);
		if (!form) {
			return false;
		}

		return FilterByForm(a_form, form);
	}

	[[nodiscard]] static bool FilterByObjectHandles(
		const RE::TESForm* a_form,
		const std::unordered_set<RE::BSScript::ObjectHandle>& a_includedObjectHandles,
		const std::unordered_set<RE::BSScript::ObjectHandle>& a_excludedObjectHandles)
	{
		if (a_includedObjectHandles.empty() && a_excludedObjectHandles.empty()) {
			return true;
		}

		if (!a_form) {
			return false;
		}

		const auto filterPredicate = [a_form](RE::BSScript::ObjectHandle a_filter) {
			return FilterByObjectHandle(a_form, a_filter);
		};

		return (a_includedObjectHandles.empty() || std::ranges::any_of(a_includedObjectHandles, filterPredicate)) &&
			   (a_excludedObjectHandles.empty() || std::ranges::none_of(a_excludedObjectHandles, filterPredicate));
	}
}

namespace Plugin::Internal::Script
{
	bool ScriptVariantFilter::VariantFilter::empty() const noexcept
	{
		return formTypes.empty() && plugins.empty() && objectHandles.empty();
	}

	void ScriptVariantFilter::VariantFilter::InsertFilter(RE::BSScript::SharedVariant a_value)
	{
		if (!a_value) {
			return;
		}

		using ValueType = RE::BSScript::TypeInfo::RawType;

		switch (a_value->GetRawType()) {
			case ValueType::kString: {
				const auto string = a_value->GetString();
				if (string.empty()) {
					break;
				}

				{
					const auto formType = RE::FORM_ENUM_STRING::GetFormTypeByString(string);
					if (formType != RE::FormType::kNone) {
						formTypes.insert(formType);
						break;
					}
				}

				{
					const auto* dataHandler = RE::TESDataHandler::GetSingleton();
					if (!dataHandler) [[unlikely]] {
						REX::Fail("Failed to get the game's data handler."sv);
					}

					const auto* plugin = dataHandler->FindLoadedFileByName(string);
					if (plugin) {
						plugins.insert(plugin);
						break;
					}
				}

				break;
			}
			case ValueType::kObject: {
				const auto object = a_value->GetObject();
				if (!object) {
					break;
				}

				const auto objectHandle = object->GetHandle();
				if (!RE::GameScript::HandlePolicy::IsFormHandle(objectHandle)) {
					break;
				}

				objectHandles.insert(objectHandle);
				break;
			}
			case ValueType::kVar: {
				const auto* var = a_value->GetVar();
				if (!var) {
					break;
				}

				InsertFilter(var);
				break;
			}
			case ValueType::kArrayString:
			case ValueType::kArrayObject:
			case ValueType::kArrayVar: {
				const auto array = a_value->GetArray();
				if (!array) {
					break;
				}

				const auto arrayLock = RE::BSAutoLock(array->elementsLock);

				for (const auto& element : *array) {
					InsertFilter(std::addressof(element));
				}

				break;
			}
			default: {
				break;
			}
		}
	}

	void ScriptVariantFilter::VariantFilter::InsertFilter(std::span<const RE::BSScript::SharedVariant> a_values)
	{
		for (const auto& value : a_values) {
			InsertFilter(value);
		}
	}

	ScriptVariantFilter::ScriptVariantFilter(
		std::span<const RE::BSScript::SharedVariant> a_includedValues,
		std::span<const RE::BSScript::SharedVariant> a_excludedValues)
	{
		_includedValues.InsertFilter(a_includedValues);
		_excludedValues.InsertFilter(a_excludedValues);
	}

	bool ScriptVariantFilter::IsMatch(const RE::TESForm* const& a_form) const
	{
		if (_includedValues.empty() && _excludedValues.empty()) {
			return true;
		}

		if (!a_form) {
			return false;
		}

		return Impl::FilterByFormTypes(a_form, _includedValues.formTypes, _excludedValues.formTypes) &&
			   Impl::FilterByPlugins(a_form, _includedValues.plugins, _excludedValues.plugins) &&
			   Impl::FilterByObjectHandles(a_form, _includedValues.objectHandles, _excludedValues.objectHandles);
	}
}
