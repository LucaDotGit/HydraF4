#include "Plugin/Papyrus/Scaleform.hpp"

#include "Plugin/Internal/Core/ScaleformUtility.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Scaleform::Impl
{
	[[nodiscard]] __forceinline static bool IsMenuOpen(const ::Scaleform::Ptr<RE::IMenu>& a_menu) noexcept
	{
		return a_menu && a_menu->menuFlags.any_atomic(RE::UI_MENU_FLAGS::kOnStack);
	}

	[[nodiscard]] __forceinline static auto GetMenu(const RE::BSFixedString& a_menuName)
		-> ::Scaleform::Ptr<RE::IMenu>
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return ui->GetMenu(a_menuName);
	}

	[[nodiscard]] __forceinline static auto GetMenuRoot(const RE::BSFixedString& a_menuName)
		-> ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>
	{
		const auto menu = GetMenu(a_menuName);
		if (!menu || !IsMenuOpen(menu)) {
			return nullptr;
		}

		const auto menuRoot = menu->uiMovie;
		if (!menuRoot) {
			return nullptr;
		}

		return menuRoot->asMovieRoot;
	}

	[[nodiscard]] static auto ToScaleformArgs(std::span<RE::BSScript::SharedVariant> a_args,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_root) -> std::optional<std::vector<::Scaleform::GFx::Value>>
	{
		if (!a_root) {
			return std::nullopt;
		}

		if (a_args.empty()) {
			return std::vector<::Scaleform::GFx::Value>();
		}

		auto gfxArgs = std::vector<::Scaleform::GFx::Value>();
		gfxArgs.reserve(a_args.size());

		for (const auto* arg : a_args) {
			if (!arg) {
				continue;
			}

			auto gfxValue = ::Plugin::Internal::Core::ScaleformUtility::FromValue(*arg, a_root);
			if (!gfxValue.has_value()) {
				continue;
			}

			gfxArgs.push_back(std::move(gfxValue).value());
		}

		return gfxArgs;
	}
}

namespace Plugin::Papyrus::Scaleform
{
	static bool IsMenuRegistered(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName)
	{
		const auto menu = Impl::GetMenu(a_menuName);
		return menu != nullptr;
	}

	static bool IsMenuOpen(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName)
	{
		const auto menuRoot = Impl::GetMenu(a_menuName);
		return Impl::IsMenuOpen(menuRoot);
	}

	static bool OpenMenu(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName)
	{
		const auto menu = Impl::GetMenu(a_menuName);
		if (menu && Impl::IsMenuOpen(menu)) {
			return false;
		}

		auto* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
		if (!uiMessageQueue) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		uiMessageQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kShow);
		return true;
	}

	static bool CloseMenu(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName)
	{
		const auto menu = Impl::GetMenu(a_menuName);
		if (menu && !Impl::IsMenuOpen(menu)) {
			return false;
		}

		auto* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
		if (!uiMessageQueue) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		uiMessageQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kHide);
		return true;
	}

	static bool ContainsVariable(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return false;
		}

		return menuRoot->IsAvailable(a_varPath.GetValue().data());
	}

	static RE::BSScript::UniqueVariant GetVariable(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return nullptr;
		}

		if (!menuRoot->IsAvailable(a_varPath.GetValue().data())) {
			return nullptr;
		}

		auto gfxValue = ::Scaleform::GFx::Value();
		if (!menuRoot->GetVariable(std::addressof(gfxValue), a_varPath.GetValue().data())) {
			return nullptr;
		}

		auto scriptVar = ::Plugin::Internal::Core::ScaleformUtility::ToVar(gfxValue, RE::BSScript::TypeInfo::RawType::kVar);
		if (!scriptVar.has_value()) {
			return nullptr;
		}

		return std::move(scriptVar).value();
	}

	static RE::BSScript::UniqueVariant GetVariableOrDefault(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath,
		RE::BSScript::SharedVariant a_default)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return RE::BSScript::CreateVariant(a_default);
		}

		if (!menuRoot->IsAvailable(a_varPath.GetValue().data())) {
			return RE::BSScript::CreateVariant(a_default);
		}

		auto gfxValue = ::Scaleform::GFx::Value();
		if (!menuRoot->GetVariable(std::addressof(gfxValue), a_varPath.GetValue().data())) {
			return RE::BSScript::CreateVariant(a_default);
		}

		auto scriptVar = ::Plugin::Internal::Core::ScaleformUtility::ToVar(gfxValue, RE::BSScript::TypeInfo::RawType::kVar);
		if (!scriptVar.has_value()) {
			return RE::BSScript::CreateVariant(a_default);
		}

		return std::move(scriptVar).value();
	}

	static bool SetVariable(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath,
		RE::BSScript::SharedVariant a_value)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return false;
		}

		if (!menuRoot->IsAvailable(a_varPath.GetValue().data())) {
			return false;
		}

		auto gfxValue = ::Plugin::Internal::Core::ScaleformUtility::FromValue(*a_value, menuRoot);
		if (!gfxValue.has_value()) {
			return false;
		}

		return menuRoot->SetVariable(a_varPath.GetValue().data(), gfxValue.value());
	}

	static RE::BSScript::UniqueVariant Invoke(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_funcName,
		std::vector<RE::BSScript::SharedVariant> a_args)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return nullptr;
		}

		if (!menuRoot->IsAvailable(a_funcName.GetValue().data())) {
			return nullptr;
		}

		auto gfxArgs = Impl::ToScaleformArgs(a_args, menuRoot);
		if (!gfxArgs.has_value()) {
			return nullptr;
		}

		auto gfxInvokeResult = ::Scaleform::GFx::Value();
		if (!menuRoot->Invoke(a_funcName.GetValue().data(), std::addressof(gfxInvokeResult), gfxArgs.value())) {
			return nullptr;
		}

		auto scriptVar = ::Plugin::Internal::Core::ScaleformUtility::ToVar(gfxInvokeResult, RE::BSScript::TypeInfo::RawType::kVar);
		if (!scriptVar.has_value()) {
			return nullptr;
		}

		return std::move(scriptVar).value();
	}

	static bool InvokeAsync(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		RE::BSFixedString a_funcName,
		std::vector<RE::BSScript::SharedVariant> a_args)
	{
		auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return false;
		}

		if (!menuRoot->IsAvailable(a_funcName.data())) {
			return false;
		}

		auto gfxArgs = Impl::ToScaleformArgs(a_args, menuRoot);
		if (!gfxArgs.has_value()) {
			return false;
		}

		F4SE::GetTaskInterface()->AddUITask([menuRoot = std::move(menuRoot), a_funcName = std::move(a_funcName), gfxArgs = std::move(gfxArgs)]() {
			if (!menuRoot) {
				return;
			}

			menuRoot->Invoke(a_funcName.data(), gfxArgs.value());
		});

		return true;
	}

	static RE::BSScript::UniqueVariant Deserialize(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath,
		RE::BSFixedString a_structName)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return nullptr;
		}

		if (!menuRoot->IsAvailable(a_varPath.GetValue().data())) {
			return nullptr;
		}

		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_vm.GetScriptStructType(a_structName, structTypeInfo) || !structTypeInfo) {
			return nullptr;
		}

		auto gfxValue = ::Scaleform::GFx::Value();
		if (!menuRoot->GetVariable(std::addressof(gfxValue), a_varPath.GetValue().data())) {
			return nullptr;
		}

		auto scriptStruct = ::Plugin::Internal::Core::ScaleformUtility::ToStruct(gfxValue, RE::BSScript::TypeInfo(structTypeInfo.get()));
		if (!scriptStruct.has_value()) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(std::move(scriptStruct).value());
	}

	static bool Serialize(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_menuName,
		::Plugin::Internal::Script::ScriptStrictString a_varPath,
		RE::BSScript::SharedVariant a_struct)
	{
		const auto menuRoot = Impl::GetMenuRoot(a_menuName);
		if (!menuRoot) {
			return false;
		}

		if (!menuRoot->IsAvailable(a_varPath.GetValue().data())) {
			return false;
		}

		const auto scriptStruct = a_struct && a_struct->IsStruct() ? a_struct->GetStruct() : nullptr;

		auto gfxValue = ::Plugin::Internal::Core::ScaleformUtility::FromStruct(scriptStruct, menuRoot);
		if (!gfxValue.has_value()) {
			return false;
		}

		return menuRoot->SetVariable(a_varPath.GetValue().data(), gfxValue.value());
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsMenuRegistered);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsMenuOpen);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, OpenMenu);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CloseMenu);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsVariable);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetVariable);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetVariableOrDefault);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetVariable);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Invoke);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, InvokeAsync);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Deserialize);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Serialize);
	}
}
