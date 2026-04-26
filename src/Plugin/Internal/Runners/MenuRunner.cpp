#include "Plugin/Internal/Runners/MenuRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Runners
{
	static constexpr auto MENUS_KEY = "menus"sv;
	static constexpr auto ASSETS_KEY = "assets"sv;
	static constexpr auto MENU_NAME_KEY = "menuName"sv;
	static constexpr auto MENU_PATH_KEY = "menuPath"sv;
	static constexpr auto MENU_ROOT_KEY = "menuRoot"sv;
	static constexpr auto MENU_FLAGS_KEY = "menuFlags"sv;
	static constexpr auto MENU_DEPTH_KEY = "menuDepth"sv;
	static constexpr auto ASSET_PATH_KEY = "assetPath"sv;
	static constexpr auto ASSET_ROOT_KEY = "assetRoot"sv;

	auto MenuRunner::MenuInfo::CreateMenuInstance() const -> REX::NotNull<std::unique_ptr<RE::GameMenuBase>>
	{
		return { std::make_unique<CustomGameMenu>() };
	}

	auto MenuRunner::MenuInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<MenuInfo, std::string>
	{
		auto rootInfo = MenuInfo();

		const auto menuNameIt = a_json.find(MENU_NAME_KEY);
		if (menuNameIt == a_json.end() || !menuNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				MENU_NAME_KEY);
		}

		const auto menuPathIt = a_json.find(MENU_PATH_KEY);
		if (menuPathIt == a_json.end() || !menuPathIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				MENU_PATH_KEY);
		}

		const auto menuRootIt = a_json.find(MENU_ROOT_KEY);
		if (menuRootIt == a_json.end() || !menuRootIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				MENU_ROOT_KEY);
		}

		const auto menuFlagsIt = a_json.find(MENU_FLAGS_KEY);
		if (menuFlagsIt != a_json.end() && !menuFlagsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				MENU_FLAGS_KEY);
		}

		const auto menuDepthIt = a_json.find(MENU_DEPTH_KEY);
		if (menuDepthIt != a_json.end() && !menuDepthIt->is_number_unsigned()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find unsigned integer value for key "{}")"sv,
				MENU_DEPTH_KEY);
		}

		rootInfo.menuName = menuNameIt->get<std::string>();
		rootInfo.menuFilePath = menuPathIt->get<std::string>();
		rootInfo.rootVarPath = menuRootIt->get<std::string>();

		if (menuFlagsIt != a_json.end()) {
			auto menuFlagSet = REX::EnumSet(RE::UI_MENU_FLAGS::kNone);

			for (auto menuFlagIt = menuFlagsIt->begin(); menuFlagIt != menuFlagsIt->end(); menuFlagIt++) {
				if (!menuFlagIt->is_string()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find string element at index {} in array for key "{}")"sv,
						std::distance(menuFlagsIt->begin(), menuFlagIt), MENU_FLAGS_KEY);
				}

				const auto flagString = menuFlagIt->get<std::string_view>();

				const auto* flagValueIt = MENU_FLAG_MAP.find(flagString);
				if (flagValueIt == MENU_FLAG_MAP.end()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find valid flag value for string "{}" in array for key "{}")"sv,
						flagString, MENU_FLAGS_KEY);
				}

				menuFlagSet.set(flagValueIt->second);
			}

			rootInfo.menuFlags = menuFlagSet;
		}

		if (menuDepthIt != a_json.end()) {
			rootInfo.menuDepth = menuDepthIt->get<std::underlying_type_t<RE::UI_DEPTH_PRIORITY>>();
		}

		return rootInfo;
	}

	auto MenuRunner::AssetInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<AssetInfo, std::string>
	{
		const auto menuNameIt = a_json.find(MENU_NAME_KEY);
		if (menuNameIt == a_json.end() || !menuNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				MENU_NAME_KEY);
		}

		const auto assetPathIt = a_json.find(ASSET_PATH_KEY);
		if (assetPathIt == a_json.end() || !assetPathIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				ASSET_PATH_KEY);
		}

		const auto assetRootIt = a_json.find(ASSET_ROOT_KEY);
		if (assetRootIt == a_json.end() || !assetRootIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				ASSET_ROOT_KEY);
		}

		auto assetInfo = AssetInfo();
		assetInfo.menuName = menuNameIt->get<std::string>();
		assetInfo.assetFilePath = assetPathIt->get<std::string>();
		assetInfo.rootVarPath = assetRootIt->get<std::string>();

		return assetInfo;
	}

	auto MenuRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto menusIt = a_json.find(MENUS_KEY);
		if (menusIt != a_json.end() && !menusIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				MENUS_KEY);
		}

		const auto assetsIt = a_json.find(ASSETS_KEY);
		if (assetsIt != a_json.end() && !assetsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				ASSETS_KEY);
		}

		auto rootInfo = RootInfo();

		if (menusIt != a_json.end()) {
			auto menus = std::vector<REX::NotNull<std::shared_ptr<MenuInfo>>>();
			menus.reserve(menusIt->size());

			for (auto menuIt = menusIt->begin(); menuIt != menusIt->end(); menuIt++) {
				if (!menuIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(menusIt->begin(), menuIt), MENUS_KEY);
				}

				auto menu = MenuInfo::FromJson(*menuIt);
				if (!menu.has_value()) {
					return std::unexpected(std::move(menu).error());
				}

				menus.emplace_back(std::make_shared<MenuInfo>(std::move(menu).value()));
			}

			rootInfo.menus = std::move(menus);
		}

		if (assetsIt != a_json.end()) {
			auto assets = std::vector<REX::NotNull<std::shared_ptr<AssetInfo>>>();
			assets.reserve(assetsIt->size());

			for (auto assetIt = assetsIt->begin(); assetIt != assetsIt->end(); assetIt++) {
				if (!assetIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(assetsIt->begin(), assetIt), ASSETS_KEY);
				}

				auto asset = AssetInfo::FromJson(*assetIt);
				if (!asset.has_value()) {
					return std::unexpected(std::move(asset).error());
				}

				assets.emplace_back(std::make_shared<AssetInfo>(std::move(asset).value()));
			}

			rootInfo.assets = std::move(assets);
		}

		return rootInfo;
	}

	MenuRunner::MenuRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension))
	{
	}

	MenuRunner::~MenuRunner() noexcept
	{
		Dispose();
	}

	auto MenuRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto filePaths = IFileRunner::LoadDirectoryFiles();
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			REX::Fail(R"(Failed to iterate directory "{}": {})"sv,
				_directoryPath.generic_string(), filePaths.error());
		}

		return a_threadPool.SubmitTaskRange(std::move(filePaths).value(), [this](const std::filesystem::path& a_filePath) {
			auto jsonRoot = IFileRunner::json_t();

			try {
				auto fileStream = std::fstream(a_filePath, std::ios::in);
				jsonRoot = IFileRunner::json_t::parse(fileStream,
					REX::Json::GetParserCallback<IFileRunner::json_t>(),
					REX::Json::ALLOW_EXCEPTIONS,
					REX::Json::IGNORE_COMMENTS);
			}
			catch (const IFileRunner::json_t::exception& error) {
				REX::LogError(R"(Failed to parse file "{}": {})"sv,
					a_filePath.generic_string(), error.what());
				return;
			}

			if (jsonRoot.empty()) {
				return;
			}

			auto rootInfo = RootInfo::FromJson(jsonRoot);
			if (!rootInfo.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					a_filePath.generic_string(), rootInfo.error());
				return;
			}

			const auto& menus = rootInfo->menus;
			const auto& assets = rootInfo->assets;

			REX::LogDebug(R"(Loaded file "{}" with {} menu(s) and {} asset(s))"sv,
				a_filePath.generic_string(),
				menus.has_value() ? menus->size() : 0,
				assets.has_value() ? assets->size() : 0);

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto MenuRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			auto& menus = rootInfo->menus;
			auto& assets = rootInfo->assets;

			if (menus.has_value()) {
				RunMenus(filePath, *menus);
			}

			if (assets.has_value()) {
				RunAssets(filePath, *assets);
			}
		}

		return {};
	}

	void MenuRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto MenuRunner::RunMenu(
		const REX::NotNull<std::shared_ptr<MenuInfo>>& a_menu)
		-> std::optional<std::string>
	{
		const auto* scaleformManager = RE::BSScaleformManager::GetSingleton();
		if (!scaleformManager) [[unlikely]] {
			REX::Fail("Failed to get the game's scaleform manager."sv);
		}

		const auto& validNameChars = scaleformManager->validNameChars;

		const auto& menuName = a_menu->menuName;
		if (menuName.find_first_not_of(validNameChars) != std::string::npos) [[unlikely]] {
			return REX::Format(R"(menu name "{}" contains invalid characters; valid characters are "{}")"sv,
				menuName, validNameChars);
		}

		const auto absoluteMenuFilePath = RE::BSScaleformManager::MOVIES_DIRECTORY_PATH / a_menu->menuFilePath;

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(absoluteMenuFilePath, isFileError)) [[unlikely]] {
			return REX::Format(R"(failed to find menu file "{}" for menu "{}")"sv,
				absoluteMenuFilePath.generic_string(), menuName);
		}

		if (!F4SE::Menus::RegisterMenu(a_menu)) [[unlikely]] {
			return REX::Format(R"(failed to register menu "{}")"sv,
				menuName);
		}

		return std::nullopt;
	}

	void MenuRunner::RunMenus(
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<MenuInfo>>> a_menus)
	{
		for (const auto& menu : a_menus) {
			const auto errorMessage = RunMenu(menu);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Registered menu "{}" from file "{}")"sv,
				menu->menuName, a_filePath.generic_string());
		}
	}

	auto MenuRunner::RunAsset(
		const REX::NotNull<std::shared_ptr<AssetInfo>>& a_asset)
		-> std::optional<std::string>
	{
		const auto& menuName = a_asset->menuName;
		const auto absoluteAssetFilePath = RE::BSScaleformManager::MOVIES_DIRECTORY_PATH / a_asset->assetFilePath;

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(absoluteAssetFilePath, isFileError)) [[unlikely]] {
			return REX::Format(R"(failed to find asset file "{}" for menu "{}")"sv,
				absoluteAssetFilePath.generic_string(), menuName);
		}

		const auto assetCallback = REX::NotNull(std::make_shared<F4SE::Menus::MenuAssetLoader>(a_asset));
		F4SE::Menus::RegisterMenuAssetLoader(assetCallback);

		return std::nullopt;
	}

	void MenuRunner::RunAssets(
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<AssetInfo>>> a_assets)
	{
		for (const auto& asset : a_assets) {
			const auto errorMessage = RunAsset(asset);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Registered asset "{}" from file "{}")"sv,
				asset->assetFilePath.generic_string(), a_filePath.generic_string());
		}
	}
}
