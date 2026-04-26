#include "Plugin/Scaleform/UI.hpp"

#include "Plugin/Internal/Core/ScaleformUtility.hpp"

namespace Plugin::Scaleform::UI
{
	class playSound final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		playSound() = default;
		~playSound() override = default;

		playSound(const playSound&) = delete;
		playSound(playSound&&) = delete;

		playSound& operator=(const playSound&) = delete;
		playSound& operator=(playSound&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto soundEditorId = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!soundEditorId.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			RE::UIUtils::PlayMenuSound(soundEditorId->data());
			paramResult->SetBoolean(true);
		}
	};

	class isMenuOpen final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		isMenuOpen() = default;
		~isMenuOpen() override = default;

		isMenuOpen(const isMenuOpen&) = delete;
		isMenuOpen(isMenuOpen&&) = delete;

		isMenuOpen& operator=(const isMenuOpen&) = delete;
		isMenuOpen& operator=(isMenuOpen&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto menuName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!menuName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto* ui = RE::UI::GetSingleton();
			if (!ui) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto isMenuOpen = ui->IsMenuOpen(menuName.value());
			if (!isMenuOpen.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			paramResult->SetBoolean(isMenuOpen.value());
		}
	};

	class openMenu final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		openMenu() = default;
		~openMenu() override = default;

		openMenu(const openMenu&) = delete;
		openMenu(openMenu&&) = delete;

		openMenu& operator=(const openMenu&) = delete;
		openMenu& operator=(openMenu&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto menuName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!menuName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto* ui = RE::UI::GetSingleton();
			if (!ui) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto isMenuOpen = ui->IsMenuOpen(menuName.value());
			if (isMenuOpen.has_value() && isMenuOpen.value()) {
				paramResult->SetBoolean(true);
				return;
			}

			auto* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
			if (!uiMessageQueue) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			uiMessageQueue->AddMessage(menuName.value(), RE::UI_MESSAGE_TYPE::kShow);
			paramResult->SetBoolean(true);
		}
	};

	class closeMenu final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		closeMenu() = default;
		~closeMenu() override = default;

		closeMenu(const closeMenu&) = delete;
		closeMenu(closeMenu&&) = delete;

		closeMenu& operator=(const closeMenu&) = delete;
		closeMenu& operator=(closeMenu&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto menuName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!menuName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto* ui = RE::UI::GetSingleton();
			if (!ui) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto isMenuOpen = ui->IsMenuOpen(menuName.value());
			if (!isMenuOpen.has_value() || !isMenuOpen.value()) {
				paramResult->SetBoolean(true);
				return;
			}

			auto* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
			if (!uiMessageQueue) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			uiMessageQueue->AddMessage(menuName.value(), RE::UI_MESSAGE_TYPE::kHide);
			paramResult->SetBoolean(true);
		}
	};

	void RegisterFunctions(::Scaleform::GFx::Movie* a_movie, ::Scaleform::GFx::Value* a_value)
	{
		if (!a_movie) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		if (!a_value) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		RE_REGISTER_SF_FUNCTION(a_movie, a_value, playSound);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, isMenuOpen);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, openMenu);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, closeMenu);
	}
}
