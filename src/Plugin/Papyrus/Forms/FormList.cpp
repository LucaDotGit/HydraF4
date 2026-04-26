#include "Plugin/Papyrus/Forms/FormList.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::FormList
{
	static std::uint32_t GetFormCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return 0;
		}

		return a_formList->GetFormCount();
	}

	static std::uint32_t GetEditorFormCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return 0;
		}

		return a_formList->GetEditorFormCount();
	}

	static std::uint32_t GetScriptFormCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return 0;
		}

		return a_formList->GetScriptFormCount();
	}

	static std::optional<std::vector<RE::TESForm*>> GetAllForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		return a_formList->GetAllForms();
	}

	static std::optional<RE::BSTArray<RE::TESForm*>> GetEditorForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		return a_formList->arrayOfForms;
	}

	static void SetEditorForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::BSTArray<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->arrayOfForms = std::move(a_values);
	}

	static std::optional<std::vector<RE::TESForm*>> GetScriptForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return std::nullopt;
		}

		return a_formList->GetScriptForms();
	}

	static void SetScriptForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->SetScriptForms(a_values);
	}

	static bool ContainsForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->ContainsForm(a_value);
	}

	static bool ContainsEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->ContainsEditorForm(a_value);
	}

	static bool ContainsScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->ContainsScriptForm(a_value);
	}

	static std::int32_t GetFormIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return -1;
		}

		const auto index = a_formList->GetFormIndex(a_value);

		if (!index.has_value()) {
			return -1;
		}

		return static_cast<std::int32_t>(index.value());
	}

	static std::int32_t GetEditorFormIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return -1;
		}

		const auto index = a_formList->GetEditorFormIndex(a_value);

		if (!index.has_value()) {
			return -1;
		}

		return static_cast<std::int32_t>(index.value());
	}

	static std::int32_t GetScriptFormIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return -1;
		}

		const auto index = a_formList->GetScriptFormIndex(a_value);

		if (!index.has_value()) {
			return -1;
		}

		return static_cast<std::int32_t>(index.value());
	}

	static RE::TESForm* GetNthForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return nullptr;
		}

		return a_formList->GetNthForm(a_index);
	}

	static bool SetNthForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->SetNthForm(a_index, a_value);
	}

	static bool RemoveNthForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->RemoveNthForm(a_index);
	}

	static RE::TESForm* GetNthEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return nullptr;
		}

		return a_formList->GetNthEditorForm(a_index);
	}

	static bool SetNthEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->SetNthEditorForm(a_index, a_value);
	}

	static bool RemoveNthEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->RemoveNthEditorForm(a_index);
	}

	static RE::TESForm* GetNthScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return nullptr;
		}

		return a_formList->GetNthScriptForm(a_index);
	}

	static bool SetNthScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->SetNthScriptForm(a_index, a_value);
	}

	static bool RemoveNthScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::uint32_t a_index)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->RemoveNthScriptForm(a_index);
	}

	static bool AddEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->AddEditorForm(a_value);
	}

	static bool RemoveEditorForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->RemoveEditorForm(a_value);
	}

	static bool AddScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->AddScriptForm(a_value);
	}

	static bool RemoveScriptForm(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		RE::TESForm* a_value)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return false;
		}

		return a_formList->RemoveScriptForm(a_value);
	}

	static void AddEditorForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->AddEditorForms(a_values);
	}

	static void RemoveEditorForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->RemoveEditorForms(a_values);
	}

	static void AddScriptForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->AddScriptForms(a_values);
	}

	static void RemoveScriptForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList,
		std::vector<RE::TESForm*> a_values)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->RemoveScriptForms(a_values);
	}

	static void ClearForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->ClearForms();
	}

	static void ClearEditorForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->ClearEditorForms();
	}

	static void ClearScriptForms(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSListForm* a_formList)
	{
		if (!a_formList) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_LIST_NULL, a_stackId);
			return;
		}

		a_formList->ClearScriptForms();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFormCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEditorFormCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptFormCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetAllForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEditorForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetEditorForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetScriptForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFormIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEditorFormIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScriptFormIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveNthScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveEditorForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveScriptForm);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddEditorForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveEditorForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, AddScriptForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, RemoveScriptForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearEditorForms);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearScriptForms);
	}
}