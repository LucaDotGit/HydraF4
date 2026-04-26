#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	class MenuRunner final
		: public IFileRunner
	{
	public:
		inline static constexpr auto DEFAULT_ROOT = "root1"sv;
		inline static constexpr auto DEFAULT_MENU_FLAGS = REX::EnumSet(RE::UI_MENU_FLAGS::kNone);
		inline static constexpr auto DEFAULT_MENU_DEPTH = REX::EnumSet(RE::UI_DEPTH_PRIORITY::kStandard);

		inline static constexpr auto MENU_FLAG_MAP = REX::StaticMap<std::string_view, RE::UI_MENU_FLAGS, 28>{
			{ "None"sv, RE::UI_MENU_FLAGS::kNone },
			{ "PauseGame"sv, RE::UI_MENU_FLAGS::kPausesGame },
			{ "IsAlwaysOpen"sv, RE::UI_MENU_FLAGS::kAlwaysOpen },
			{ "UseCursor"sv, RE::UI_MENU_FLAGS::kUsesCursor },
			{ "UseMenuContext"sv, RE::UI_MENU_FLAGS::kUsesMenuContext },
			{ "IsModal"sv, RE::UI_MENU_FLAGS::kModal },
			{ "FreezeFrameBackground"sv, RE::UI_MENU_FLAGS::kFreezeFrameBackground },
			// { "IsOnStack"sv, RE::UI_MENU_FLAGS::kOnStack }, // indicates an active/open menu, which should only be set by the engine
			{ "DisablePauseMenu"sv, RE::UI_MENU_FLAGS::kDisablePauseMenu },
			{ "RequireUpdate"sv, RE::UI_MENU_FLAGS::kRequiresUpdate },
			{ "IsTopmostRenderedMenu"sv, RE::UI_MENU_FLAGS::kTopmostRenderedMenu },
			{ "UpdateUsesCursor"sv, RE::UI_MENU_FLAGS::kUpdateUsesCursor },
			{ "AllowSaving"sv, RE::UI_MENU_FLAGS::kAllowSaving },
			{ "RenderOffscreenTargets"sv, RE::UI_MENU_FLAGS::kRendersOffscreenTargets },
			{ "IsInventoryItemMenu"sv, RE::UI_MENU_FLAGS::kInventoryItemMenu },
			{ "DoNotHideCursorWhenTopmost"sv, RE::UI_MENU_FLAGS::kDoNotHideCursorWhenTopmost },
			{ "UseCustomRendering"sv, RE::UI_MENU_FLAGS::kCustomRendering },
			{ "AssignCursorToRenderer"sv, RE::UI_MENU_FLAGS::kAssignCursorToRenderer },
			{ "IsApplicationMenu"sv, RE::UI_MENU_FLAGS::kApplicationMenu },
			{ "HasButtonBar"sv, RE::UI_MENU_FLAGS::kHasButtonBar },
			{ "IsTopButtonBar"sv, RE::UI_MENU_FLAGS::kIsTopButtonBar },
			{ "AdvanceUnderPauseMenu"sv, RE::UI_MENU_FLAGS::kAdvancesUnderPauseMenu },
			{ "RenderUnderPauseMenu"sv, RE::UI_MENU_FLAGS::kRendersUnderPauseMenu },
			{ "UseBlurredBackground"sv, RE::UI_MENU_FLAGS::kUsesBlurredBackground },
			{ "AllowCompanionApp"sv, RE::UI_MENU_FLAGS::kCompanionAppAllowed },
			{ "FreezeFramePause"sv, RE::UI_MENU_FLAGS::kFreezeFramePause },
			{ "SkipRenderDuringFreezeFrameScreenshot"sv, RE::UI_MENU_FLAGS::kSkipRenderDuringFreezeFrameScreenshot },
			{ "UseLargeScaleformRenderCacheMode"sv, RE::UI_MENU_FLAGS::kLargeScaleformRenderCacheMode },
			{ "UseMovementToDirection"sv, RE::UI_MENU_FLAGS::kUsesMovementToDirection }
		};

		class MenuInfo final
			: public F4SE::Menus::MenuInfo
		{
		public:
			MenuInfo() = default;
			~MenuInfo() noexcept override = default;

			MenuInfo(const MenuInfo&) = default;
			MenuInfo(MenuInfo&&) noexcept = default;

			MenuInfo& operator=(const MenuInfo&) = default;
			MenuInfo& operator=(MenuInfo&&) noexcept = default;

			[[nodiscard]] auto CreateMenuInstance() const -> REX::NotNull<std::unique_ptr<RE::GameMenuBase>> override;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<MenuInfo, std::string>;
		};

		class AssetInfo final
			: public F4SE::Menus::MenuAssetInfo
		{
		public:
			AssetInfo() = default;
			~AssetInfo() noexcept override = default;

			AssetInfo(const AssetInfo&) = default;
			AssetInfo(AssetInfo&&) noexcept = default;

			AssetInfo& operator=(const AssetInfo&) = default;
			AssetInfo& operator=(AssetInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<AssetInfo, std::string>;
		};

		class RootInfo final
			: public std::enable_shared_from_this<RootInfo>
		{
		public:
			RootInfo() = default;
			~RootInfo() noexcept = default;

			RootInfo(const RootInfo&) = default;
			RootInfo(RootInfo&&) noexcept = default;

			RootInfo& operator=(const RootInfo&) = default;
			RootInfo& operator=(RootInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<RootInfo, std::string>;

			std::optional<std::vector<REX::NotNull<std::shared_ptr<MenuInfo>>>> menus;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<AssetInfo>>>> assets;
		};

		MenuRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		~MenuRunner() noexcept override;

		MenuRunner(const MenuRunner&) = delete;
		MenuRunner(MenuRunner&&) = delete;

		MenuRunner& operator=(const MenuRunner&) = delete;
		MenuRunner& operator=(MenuRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		class CustomGameMenu final
			: public RE::GameMenuBase
		{
		public:
			CustomGameMenu() = default;
			~CustomGameMenu() noexcept override = default;

			CustomGameMenu(const CustomGameMenu&) = delete;
			CustomGameMenu(CustomGameMenu&&) = delete;

			CustomGameMenu& operator=(const CustomGameMenu&) = delete;
			CustomGameMenu& operator=(CustomGameMenu&&) = delete;
		};

		static auto RunMenu(
			const REX::NotNull<std::shared_ptr<MenuInfo>>& a_menu)
			-> std::optional<std::string>;
		static void RunMenus(
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<MenuInfo>>> a_menus);

		static auto RunAsset(
			const REX::NotNull<std::shared_ptr<AssetInfo>>& a_asset)
			-> std::optional<std::string>;
		static void RunAssets(
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<AssetInfo>>> a_assets);

		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
