#include "Plugin/Papyrus/Forms/Form.hpp"

// NOLINTBEGIN(bugprone-suspicious-include)
#include "Plugin/Papyrus/Forms/Form_Parent.cpp"
#include "Plugin/Papyrus/Forms/Form_Search.cpp"
// NOLINTEND(bugprone-suspicious-include)

#include "Plugin/Internal/Game/GameEditorIds.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::Forms::Form
{
	using FormFlags = RE::FormFlags;
	using FormChangeFlags = RE::TESForm::ChangeFlags;

	static RE::TESForm* Create(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::FormType a_type)
	{
		if (a_type < RE::FormType::kNone || a_type >= RE::FormType::kTotal) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TYPE_ID_OUT_OF_RANGE, a_stackId,
				std::to_underlying(a_type), std::to_underlying(RE::FormType::kNone), std::to_underlying(RE::FormType::kTotal));
			return nullptr;
		}

		auto factories = RE::IFormFactory::GetFormFactories();
		auto* factory = factories[std::to_underlying(a_type)];

		if (!factory) {
			return nullptr;
		}

		return factory->DoCreate();
	}

	static RE::TESForm* CreateAndCopy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return nullptr;
		}

		const auto formFactories = RE::IFormFactory::GetFormFactories();
		const auto formType = a_form->formType.underlying();

		auto* formFactory = formFactories[formType];
		if (!formFactory) {
			return nullptr;
		}

		auto* copy = formFactory->DoCreate();
		if (!copy) {
			return nullptr;
		}

		copy->Copy(a_form);
		return copy;
	}

	static bool CopyTo(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::TESForm* a_copy)
	{
		if (!a_form || !a_copy) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		if (a_form->formType != a_copy->formType) [[unlikely]] {
			a_vm.PostError("The specified forms are not of the same type"sv, a_stackId);
			return false;
		}

		const auto formFactories = RE::IFormFactory::GetFormFactories();
		const auto formType = a_form->formType.underlying();

		auto* formFactory = formFactories[formType];
		if (!formFactory) {
			return false;
		}

		a_copy->Copy(a_form);
		return true;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		FormFlags a_flag)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return a_form->formFlags.any_atomic(a_flag);
	}

	static FormFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return FormFlags::kNone;
		}

		return a_form->formFlags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		FormFlags a_flags)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return;
		}

		a_form->formFlags.store_atomic(a_flags);
		a_form->AddChange(FormChangeFlags::kFlags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		FormFlags a_flag,
		bool a_set)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return;
		}

		a_form->formFlags.set_atomic(a_set, a_flag);
		a_form->AddChange(FormChangeFlags::kFlags);
	}

	static FormFlags GetRecordFlag_IsDeleted(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FormFlags::kDeleted; }
	static FormFlags GetRecordFlag_IsIgnored(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return FormFlags::kIgnored; }

	static bool GetIsDeleted(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return a_form->formFlags.any_atomic(FormFlags::kDeleted);
	}

	static void SetIsDeleted(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		bool a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return;
		}

		a_form->formFlags.set_atomic(a_value, FormFlags::kDeleted);
		a_form->AddChange(FormChangeFlags::kFlags);
	}

	static bool GetIsIgnored(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return a_form->formFlags.any_atomic(FormFlags::kIgnored);
	}

	static void SetIsIgnored(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		bool a_value)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return;
		}

		a_form->formFlags.set_atomic(a_value, FormFlags::kIgnored);
		a_form->AddChange(FormChangeFlags::kFlags);
	}

	static bool GetIsCreated(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return a_form->IsCreated();
	}

	static RE::TESFormID GetFormId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		return a_form->GetFormID();
	}

	static RE::TESFormID GetLocalFormId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		return a_form->GetLocalFormID();
	}

	static RE::BSFixedString GetEditorId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		auto editorId = ::Plugin::GetInternalManager()->GetGameManager()->GetEditorIds()->GetFormEditorId(a_form);
		if (!editorId.has_value()) {
			return {};
		}

		return std::move(editorId).value();
	}

	static bool SetEditorId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		RE::BSFixedString a_editorId)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		return ::Plugin::GetInternalManager()->GetGameManager()->GetEditorIds()->SetFormEditorId(a_form, a_editorId);
	}

	static RE::BSFixedString GetScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		auto objectType = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_vm.GetScriptObjectType(a_form->GetFormType(), objectType) || !objectType) {
			return {};
		}

		return objectType->GetName();
	}

	static RE::FormType GetTypeId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return RE::FormType::kNone;
		}

		return a_form->formType.get_atomic();
	}

	static RE::FormType GetTypeIdByString(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_typeString)
	{
		return RE::FORM_ENUM_STRING::GetFormTypeByString(a_typeString);
	}

	static std::string_view GetTypeString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		return a_form->GetFormTypeString();
	}

	static std::string_view GetTypeStringById(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::FormType a_typeId)
	{
		if (a_typeId < RE::FormType::kNone || a_typeId >= RE::FormType::kTotal) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::TYPE_ID_OUT_OF_RANGE, a_stackId,
				std::to_underlying(a_typeId), std::to_underlying(RE::FormType::kNone), std::to_underlying(RE::FormType::kTotal));
			return {};
		}

		return RE::FORM_ENUM_STRING::GetFormStringByType(a_typeId);
	}

	static bool IsInPlugin(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::string_view a_pluginName)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return false;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto* plugin = dataHandler->FindLoadedFileByName(a_pluginName);
		if (!plugin) {
			return false;
		}

		return plugin->IsFormInFile(a_form->GetFormID());
	}

	static std::uint32_t GetPluginCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return 0;
		}

		const auto* pluginArray = a_form->sourceFiles.array;
		if (!pluginArray) {
			return 0;
		}

		return pluginArray->size();
	}

	static std::int32_t GetPluginIndexByName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::string_view a_pluginName)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return -1;
		}

		const auto* pluginArray = a_form->sourceFiles.array;
		if (!pluginArray) {
			return -1;
		}

		const auto* pluginIt = std::ranges::find_if(*pluginArray, [a_pluginName](const RE::TESFile* a_file) {
			if (!a_file) {
				return false;
			}

			const auto fileName = std::string_view(a_file->filename.data());
			return REX::EqualsIgnoreCase(fileName, a_pluginName);
		});

		if (pluginIt == pluginArray->end()) {
			return -1;
		}

		return static_cast<std::int32_t>(std::distance(pluginArray->begin(), pluginIt));
	}

	static std::string_view GetNthPluginName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESForm* a_form,
		std::int32_t a_index)
	{
		if (!a_form) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return {};
		}

		const auto* plugin = a_form->GetFile(a_index);
		if (!plugin) {
			return {};
		}

		return plugin->filename.data();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Create);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, CreateAndCopy);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, CopyTo);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsDeleted);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsIgnored);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsDeleted);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsDeleted);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsIgnored);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetIsIgnored);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetIsCreated);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFormId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLocalFormId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEditorId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEditorId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTypeId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTypeIdByString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTypeString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTypeStringById);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsInPlugin);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPluginCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPluginIndexByName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthPluginName);

		Form::Parent::RegisterFunctions(a_vm);
		Form::Search::RegisterFunctions(a_vm);
	}
}