#include "Plugin/Papyrus/Forms/Form.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptVariantFilter.hpp"

namespace Plugin::Papyrus::Forms::Form::Search
{
	using ForEachResultType = RE::BSContainer::ForEachResult;

	static std::optional<std::vector<RE::TESForm*>> GetAllByType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::FormType a_type,
		std::string_view a_pluginName)
	{
		if (a_type < RE::FormType::kNone || a_type >= RE::FormType::kTotal) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TYPE_ID_OUT_OF_RANGE, a_stackId,
				std::to_underlying(a_type), std::to_underlying(RE::FormType::kNone), std::to_underlying(RE::FormType::kTotal));
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto* plugin = dataHandler->FindLoadedFileByName(a_pluginName);
		const auto pluginFilter = [plugin](const RE::TESForm* a_form) {
			return a_form && plugin && plugin->IsFormInFile(a_form->GetFormID());
		};

		if (a_type == RE::TESObjectCELL::FORM_TYPE) {
			const auto& cells = dataHandler->interiorCells;
			if (a_pluginName.empty()) {
				return std::vector<RE::TESForm*>{ cells.begin(), cells.end() };
			}

			auto result = std::vector<RE::TESForm*>();
			std::ranges::copy_if(cells, std::back_inserter(result), pluginFilter);
			return result;
		}

		const auto& formArray = dataHandler->formArrays[std::to_underlying(a_type)];
		if (a_pluginName.empty()) {
			return std::vector<RE::TESForm*>{ formArray.begin(), formArray.end() };
		}

		auto result = std::vector<RE::TESForm*>();
		std::ranges::copy_if(formArray, std::back_inserter(result), pluginFilter);
		return result;
	}

	static std::vector<RE::TESForm*> FindAllByType(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		RE::FormType a_type)
	{
		auto result = std::vector<RE::TESForm*>();
		std::ranges::copy_if(a_forms, std::back_inserter(result), [a_type](const RE::TESForm* a_form) {
			return a_form && a_form->formType.get_atomic() == a_type;
		});

		return result;
	}

	static std::optional<std::vector<RE::TESForm*>> FindAllByPluginName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		std::string_view a_pluginName)
	{
		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto* plugin = dataHandler->FindLoadedFileByName(a_pluginName);
		if (!plugin) {
			return std::nullopt;
		}

		auto result = std::vector<RE::TESForm*>();
		std::ranges::copy_if(a_forms, std::back_inserter(result), [plugin](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			return plugin->IsFormInFile(a_form->GetFormID());
		});

		return result;
	}

	static std::vector<RE::TESForm*> FindAllByFlag(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		RE::FormFlags a_flag,
		bool a_isSet)
	{
		auto result = std::vector<RE::TESForm*>();
		std::ranges::copy_if(a_forms, std::back_inserter(result), [a_flag, a_isSet](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			return a_form->formFlags.any_atomic(a_flag) == a_isSet;
		});

		return result;
	}

	static bool ContainsAnyByFilter(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		return std::ranges::any_of(a_forms, [&scriptFilter](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			return scriptFilter.IsMatch(a_form);
		});
	}

	static bool ContainsAllByFilter(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		return std::ranges::all_of(a_forms, [&scriptFilter](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			return scriptFilter.IsMatch(a_form);
		});
	}

	static std::vector<RE::TESForm*> FindAllByFilter(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<RE::TESForm*> a_forms,
		std::vector<RE::BSScript::SharedVariant> a_includedObjects,
		std::vector<RE::BSScript::SharedVariant> a_excludedObjects)
	{
		const auto scriptFilter = ::Plugin::Internal::Script::ScriptVariantFilter(a_includedObjects, a_excludedObjects);

		auto result = std::vector<RE::TESForm*>();
		std::ranges::copy_if(a_forms, std::back_inserter(result), [&scriptFilter](const RE::TESForm* a_form) {
			if (!a_form) {
				return false;
			}

			return scriptFilter.IsMatch(a_form);
		});

		return result;
	}

	static bool ContainsKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::ContainsKeyword(a_form, a_keyword).value_or(false);
	}

	static bool ContainsAnyKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BGSKeyword*> a_keywords)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::any_of(a_keywords, [a_form](const RE::BGSKeyword* a_keyword) {
			return RE::TESForm::ContainsKeyword(a_form, a_keyword).value_or(false);
		});
	}

	static bool ContainsAllKeywords(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BGSKeyword*> a_keywords)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::all_of(a_keywords, [a_form](const RE::BGSKeyword* a_keyword) {
			return RE::TESForm::ContainsKeyword(a_form, a_keyword).value_or(false);
		});
	}

	static bool ContainsPartialKeywordString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BSFixedString a_keywordID)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
			return a_keyword && a_keyword->formEditorID.contains(a_keywordID) ?
					   ForEachResultType::kStop :
					   ForEachResultType::kContinue;
		}) == ForEachResultType::kStop;
	}

	static bool ContainsExactKeywordString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BSFixedString a_keywordID)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
			return a_keyword && a_keyword->formEditorID == a_keywordID ?
					   ForEachResultType::kStop :
					   ForEachResultType::kContinue;
		}) == ForEachResultType::kStop;
	}

	static bool ContainsAnyPartialKeywordString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BSFixedString> a_keywordIDs)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::any_of(a_keywordIDs, [a_form](const RE::BSFixedString& a_keywordID) {
			return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
				return a_keyword && a_keyword->formEditorID.contains(a_keywordID) ?
						   ForEachResultType::kStop :
						   ForEachResultType::kContinue;
			}) == ForEachResultType::kStop;
		});
	}

	static bool ContainsAllPartialKeywordStrings(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BSFixedString> a_keywordIDs)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::all_of(a_keywordIDs, [a_form](const RE::BSFixedString& a_keywordID) {
			return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
				return a_keyword && a_keyword->formEditorID.contains(a_keywordID) ?
						   ForEachResultType::kStop :
						   ForEachResultType::kContinue;
			}) == ForEachResultType::kStop;
		});
	}

	static bool ContainsAnyExactKeywordString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BSFixedString> a_keywordIDs)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::any_of(a_keywordIDs, [a_form](const RE::BSFixedString& a_keywordID) {
			return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
				return a_keyword && a_keyword->formEditorID == a_keywordID ?
						   ForEachResultType::kStop :
						   ForEachResultType::kContinue;
			}) == ForEachResultType::kStop;
		});
	}

	static bool ContainsAllExactKeywordStrings(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::vector<RE::BSFixedString> a_keywordIDs)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (!RE::TESForm::CanHoldKeywords(a_form)) {
			return false;
		}

		return std::ranges::all_of(a_keywordIDs, [a_form](const RE::BSFixedString& a_keywordID) {
			return RE::TESForm::ForEachKeyword(a_form, [&a_keywordID](RE::BGSKeyword* a_keyword) {
				return a_keyword && a_keyword->formEditorID == a_keywordID ?
						   ForEachResultType::kStop :
						   ForEachResultType::kContinue;
			}) == ForEachResultType::kStop;
		});
	}

	// NOLINTNEXTLINE(misc-use-internal-linkage)
	inline void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAllByType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllByType);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllByPluginName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllByFlag);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAnyByFilter);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAllByFilter);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, FindAllByFilter);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAnyKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAllKeywords);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsPartialKeywordString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsExactKeywordString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAnyPartialKeywordString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAllPartialKeywordStrings);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAnyExactKeywordString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsAllExactKeywordStrings);
	}
}
