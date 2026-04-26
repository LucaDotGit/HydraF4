#include "Plugin/Papyrus/UI.hpp"

#include "Plugin/Internal/Game/GameUIColors.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"
#include "Plugin/Internal/Structs/FunctionRefs.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::UI::Impl
{
	class MessageBoxCallback final
		: public RE::IMessageBoxCallback
	{
	public:
		explicit MessageBoxCallback(::Plugin::Internal::Script::ScriptFunctionRef a_functionRef)
			: _functionRef(std::move(a_functionRef))
		{
		}

		~MessageBoxCallback() noexcept override = default;

		MessageBoxCallback(const MessageBoxCallback&) = delete;
		MessageBoxCallback(MessageBoxCallback&&) = delete;

		MessageBoxCallback& operator=(const MessageBoxCallback&) = delete;
		MessageBoxCallback& operator=(MessageBoxCallback&&) = delete;

		void operator()(std::uint8_t a_buttonIndex) override
		{
			_functionRef.InvokeFunction(a_buttonIndex);
		}

	private:
		::Plugin::Internal::Script::ScriptFunctionRef _functionRef;
	};

	template <class T>
	[[nodiscard]] __forceinline static bool IsMenuOpen(const ::Scaleform::Ptr<T>& a_menu)
		requires(std::derived_from<T, RE::IMenu> &&
				 !std::is_pointer_v<T> &&
				 !std::is_reference_v<T>)
	{
		return a_menu ? a_menu->menuFlags.any_atomic(RE::UI_MENU_FLAGS::kOnStack) : false;
	}
}

namespace Plugin::Papyrus::UI
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;
	using FunctionRefStruct = ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData;

	static RE::ObjectRefHandle GetContainerMenuContainerRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto containerMenu = ui->GetMenu<RE::ContainerMenu>();
		if (Impl::IsMenuOpen(containerMenu)) {
			return containerMenu->containerRef;
		}

		const auto barterMenu = ui->GetMenu<RE::BarterMenu>();
		if (Impl::IsMenuOpen(barterMenu)) {
			return barterMenu->containerRef;
		}

		return nullptr;
	}

	static RE::NiPointer<RE::TESObjectREFR> GetWorkbenchMenuContainerRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto examineMenu = ui->GetMenu<RE::ExamineMenu>();
		if (Impl::IsMenuOpen(examineMenu)) {
			return examineMenu->workbenchContainerRef;
		}

		const auto powerArmorMenu = ui->GetMenu<RE::PowerArmorModMenu>();
		if (Impl::IsMenuOpen(powerArmorMenu)) {
			return powerArmorMenu->workbenchContainerRef;
		}

		return nullptr;
	}

	static RE::NiPointer<RE::TESObjectREFR> GetWorkbenchMenuWorkbenchRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto examineMenu = ui->GetMenu<RE::ExamineMenu>();
		if (Impl::IsMenuOpen(examineMenu)) {
			return examineMenu->workbenchRef;
		}

		const auto powerArmorMenu = ui->GetMenu<RE::PowerArmorModMenu>();
		if (Impl::IsMenuOpen(powerArmorMenu)) {
			return powerArmorMenu->workbenchRef;
		}

		return nullptr;
	}

	static RE::ObjectRefHandle GetBarterMenuChestRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto menu = ui->GetMenu<RE::BarterMenu>();
		if (!Impl::IsMenuOpen(menu)) {
			return nullptr;
		}

		return menu->vendorChestRef;
	}

	static RE::NiPointer<RE::Actor> GetBarterMenuMerchant(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto menu = ui->GetMenu<RE::BarterMenu>();
		if (!Impl::IsMenuOpen(menu)) {
			return nullptr;
		}

		const auto merchantRefHandle = menu->vendorActor;
		if (!merchantRefHandle) {
			return nullptr;
		}

		const auto merchantRef = merchantRefHandle.get();
		if (!merchantRef) {
			return nullptr;
		}

		return RE::dynamic_ni_pointer_cast<RE::Actor>(merchantRef);
	}

	static std::int32_t GetSelectedQuickContainerItemIndex(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* hudDataModel = RE::HUDDataModel::GetSingleton();
		if (!hudDataModel) [[unlikely]] {
			REX::Assert(false);
			return -1;
		}

		return hudDataModel->rolloverModel.quickContainerModel.selectedItemIndex;
	}

	static void TakeSelectedQuickContainerItem(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto* hudDataModel = RE::HUDDataModel::GetSingleton();
		if (!hudDataModel) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		hudDataModel->rolloverModel.quickContainerModel.TakeSelectedItem();
	}

	static void PlayMenuSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_sound) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SOUND_NULL, a_stackId);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([a_sound]() {
			RE::UIUtils::PlayMenuSound(a_sound->GetFormEditorID());
		});
	}

	static void PlayMenuSoundByEditorId(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_soundEditorId)
	{
		F4SE::GetTaskInterface()->AddTask([a_soundEditorId = std::move(a_soundEditorId)]() {
			RE::UIUtils::PlayMenuSound(a_soundEditorId.data());
		});
	}

	static void PlayPipBoySound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_sound) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SOUND_NULL, a_stackId);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([a_sound]() {
			RE::UIUtils::PlayPipboySound(a_sound->GetFormEditorID());
		});
	}

	static void PlayPipBoySoundByEditorId(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_soundEditorId)
	{
		F4SE::GetTaskInterface()->AddTask([a_soundEditorId = std::move(a_soundEditorId)]() {
			RE::UIUtils::PlayPipboySound(a_soundEditorId.data());
		});
	}

	static ColorStruct GetHudColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto color = ::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->GetHudRgb();
		return { .red = color.red, .green = color.green, .blue = color.blue };
	}

	static void SetHudColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		const auto rgb = ::Plugin::Internal::Game::GameUIColors::RgbData{
			.red = a_color->red,
			.green = a_color->green,
			.blue = a_color->blue
		};

		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SetHudRgb(rgb);
	}

	static void SaveHudColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SaveHudColorAsync();
	}

	static ColorStruct GetPipBoyUIColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto color = ::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->GetPipBoyRgb();
		return { .red = color.red, .green = color.green, .blue = color.blue };
	}

	static void SetPipBoyUIColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		const auto rgb = ::Plugin::Internal::Game::GameUIColors::RgbData{
			.red = a_color->red,
			.green = a_color->green,
			.blue = a_color->blue
		};

		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SetPipBoyRgb(rgb);
	}

	static void SavePipBoyUIColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SavePipBoyColorAsync();
	}

	static ColorStruct GetPowerArmorUIColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto color = ::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->GetPowerArmorRgb();
		return { .red = color.red, .green = color.green, .blue = color.blue };
	}

	static void SetPowerArmorUIColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		const auto rgb = ::Plugin::Internal::Game::GameUIColors::RgbData{
			.red = a_color->red,
			.green = a_color->green,
			.blue = a_color->blue
		};

		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SetPowerArmorRgb(rgb);
	}

	static void SavePowerArmorUIColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SavePowerArmorColorAsync();
	}

	static ColorStruct GetWorkbenchHighlightColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto color = ::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->GetWorkbenchHighlightRgb();
		return { .red = color.red, .green = color.green, .blue = color.blue };
	}

	static void SetWorkbenchHighlightColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		const auto rgb = ::Plugin::Internal::Game::GameUIColors::RgbData{
			.red = a_color->red,
			.green = a_color->green,
			.blue = a_color->blue
		};

		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SetWorkbenchHighlightRgb(rgb);
	}

	static void SaveWorkbenchHighlightColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SaveWorkbenchHighlightColorAsync();
	}

	static ColorStruct GetPowerArmorWorkbenchHighlightColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto color = ::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->GetPowerArmorWorkbenchHighlightRgb();
		return { .red = color.red, .green = color.green, .blue = color.blue };
	}

	static void SetPowerArmorWorkbenchHighlightColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<ColorStruct> a_color)
	{
		if (!a_color.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		const auto rgb = ::Plugin::Internal::Game::GameUIColors::RgbData{
			.red = a_color->red,
			.green = a_color->green,
			.blue = a_color->blue
		};

		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SetPowerArmorWorkbenchHighlightRgb(rgb);
	}

	static void SavePowerArmorWorkbenchHighlightColor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		::Plugin::GetInternalManager()->GetGameManager()->GetUIColors()->SavePowerArmorWorkbenchHighlightColorAsync();
	}

	static void ShowBasicMessageBox(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_text)
	{
		auto* messageManager = RE::MessageMenuManager::GetSingleton();
		if (!messageManager) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		messageManager->CreateOKMessage(a_text.data());
	}

	static void ShowBasicNotification(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_text)
	{
		F4SE::GetTaskInterface()->AddUITask([a_text = std::move(a_text)]() {
			RE::SendHUDMessage::ShowHUDMessage(a_text.data());
		});
	}

	static void ShowCustomMessageBox(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::optional<FunctionRefStruct> a_functionRef,
		::Plugin::Internal::Script::ScriptStrictString a_text,
		std::vector<::Plugin::Internal::Script::ScriptStrictString> a_buttons)
	{
		if (!a_functionRef.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		auto scriptFunctionRef = ::Plugin::Internal::Script::ScriptFunctionRef(a_functionRef.value());
		if (!scriptFunctionRef.ValidateRefMembers()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_INVALID, a_stackId);
			return;
		}

		const auto callbackArgs = std::array{ RE::BSScript::TypeInfo(RE::BSScript::TypeInfo::RawType::kInt) };
		if (!scriptFunctionRef.IsFunctionOfParamTypes(callbackArgs)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_FUNCTION_REF_NON_INVOKABLE, a_stackId);
			return;
		}

		auto* messageManager = RE::MessageMenuManager::GetSingleton();
		if (!messageManager) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto buttonCount = static_cast<std::uint32_t>(std::min(a_buttons.size(), static_cast<std::size_t>(RE::MessageMenuManager::MAX_BUTTON_COUNT)));

		auto newButtons = RE::BSTArray<RE::BSString>();
		newButtons.reserve(buttonCount);

		for (auto i = 0ui32; i < buttonCount; i++) {
			newButtons.emplace_back(a_buttons[i].GetValue());
		}

		auto messageCallback = REX::NotNull(std::make_unique<Impl::MessageBoxCallback>(std::move(scriptFunctionRef)));
		messageManager->CreateCustomMessage(a_text.GetValue().data(), (*messageCallback).release(), std::move(newButtons));
	}

	static void ShowCustomNotification(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_text,
		RE::BSFixedString a_soundEditorId,
		bool a_throttle)
	{
		F4SE::GetTaskInterface()->AddUITask([a_text = std::move(a_text), a_soundEditorId = std::move(a_soundEditorId), a_throttle]() {
			RE::SendHUDMessage::ShowHUDMessage(a_text.data(), a_soundEditorId.data(), a_throttle);
		});
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetContainerMenuContainerRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWorkbenchMenuContainerRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWorkbenchMenuWorkbenchRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBarterMenuChestRef);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBarterMenuMerchant);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSelectedQuickContainerItemIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, TakeSelectedQuickContainerItem);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PlayMenuSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PlayMenuSoundByEditorId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PlayPipBoySound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, PlayPipBoySoundByEditorId);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetHudColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetHudColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveHudColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPipBoyUIColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPipBoyUIColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SavePipBoyUIColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPowerArmorUIColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPowerArmorUIColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SavePowerArmorUIColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPowerArmorWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPowerArmorWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SavePowerArmorWorkbenchHighlightColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ShowBasicMessageBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ShowBasicNotification);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ShowCustomMessageBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ShowCustomNotification);
	}
}
