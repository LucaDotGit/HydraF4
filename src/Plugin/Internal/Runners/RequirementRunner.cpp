#include "Plugin/Internal/Runners/RequirementRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto NAME_KEY = "name"sv;
	static constexpr auto VERSION_KEY = "version"sv;
	static constexpr auto REQUIREMENTS_KEY = "requirements"sv;
	static constexpr auto GAME_VERSION_KEY = "gameVersion"sv;
	static constexpr auto GAME_PLUGINS_KEY = "gamePlugins"sv;
	static constexpr auto XSE_VERSION_KEY = "xseVersion"sv;
	static constexpr auto XSE_PLUGINS_KEY = "xsePlugins"sv;
	static constexpr auto MODULES_KEY = "modules"sv;
	static constexpr auto FILE_VERSION_KEY = "fileVersion"sv;
	static constexpr auto PRODUCT_VERSION_KEY = "productVersion"sv;

	static_assert(
		RequirementRunner::ErrorMessageBoxCallback::DEFAULT_BUTTON_ARRAY[RequirementRunner::ErrorMessageBoxCallback::ButtonIndex::Abort] ==
		RequirementRunner::ErrorMessageBoxCallback::ABORT_TRANSLATION_KEY);

	static_assert(
		RequirementRunner::ErrorMessageBoxCallback::CANCELABLE_BUTTON_ARRAY[RequirementRunner::ErrorMessageBoxCallback::ButtonIndex::Abort] ==
		RequirementRunner::ErrorMessageBoxCallback::ABORT_TRANSLATION_KEY);

	static_assert(
		RequirementRunner::ErrorMessageBoxCallback::CANCELABLE_BUTTON_ARRAY[RequirementRunner::ErrorMessageBoxCallback::ButtonIndex::Cancel] ==
		RequirementRunner::ErrorMessageBoxCallback::CANCEL_TRANSLATION_KEY);

	RequirementRunner::ErrorMessageBoxCallback::ErrorMessageBoxCallback(bool a_hasCancelButton)
		: _hasCancelButton(a_hasCancelButton)
	{
		RegisterEvents();
	}

	RequirementRunner::ErrorMessageBoxCallback::~ErrorMessageBoxCallback() noexcept
	{
		REX::TryOrFail<std::exception>([this]() {
			UnregisterEvents();
		});

		_menuOpenSignal.release();
		_buttonClickSignal.release();
	}

	void RequirementRunner::ErrorMessageBoxCallback::operator()(std::uint8_t a_buttonIndex)
	{
		switch (a_buttonIndex) {
			case ButtonIndex::Abort: {
				std::quick_exit(EXIT_SUCCESS);
				break;
			}
			case ButtonIndex::Cancel: {
				REX::Assert(_hasCancelButton);
				_buttonClickSignal.release();
				break;
			}
			[[unlikely]] default: {
				REX::Assert(false);
				break;
			}
		}
	}

	RE::BSEventNotifyControl RequirementRunner::ErrorMessageBoxCallback::ProcessEvent(const RE::MenuOpenCloseEvent& a_event,
		[[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (!a_event.opening || a_event.menuName != RE::MessageBoxMenu::MENU_NAME) {
			return RE::BSEventNotifyControl::kContinue;
		}

		_menuOpenSignal.release();
		return RE::BSEventNotifyControl::kContinue;
	}

	bool RequirementRunner::ErrorMessageBoxCallback::TryWaitForMenuOpen() noexcept
	{
		return _menuOpenSignal.try_acquire_for(MENU_OPEN_TIMEOUT_DELAY);
	}

	void RequirementRunner::ErrorMessageBoxCallback::WaitForButtonClick() noexcept
	{
		_buttonClickSignal.acquire();
	}

	void RequirementRunner::ErrorMessageBoxCallback::RegisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuOpenCloseEvent>*>(ui);
		if (!eventSource) [[unlikely]] {
			return;
		}

		eventSource->RegisterSink(this);
	}

	void RequirementRunner::ErrorMessageBoxCallback::UnregisterEvents()
	{
		auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			return;
		}

		auto* eventSource = static_cast<RE::BSTEventSource<RE::MenuOpenCloseEvent>*>(ui);
		if (!eventSource) [[unlikely]] {
			return;
		}

		eventSource->UnregisterSink(this);
	}

	auto RequirementRunner::ErrorMessageBoxCallback::CreateMessage(REX::zstring_view a_message, bool a_addCancelButton)
		-> REX::NotNull<RE::BSTSmartPointer<ErrorMessageBoxCallback>>
	{
		auto* messageManager = RE::MessageMenuManager::GetSingleton();
		if (!messageManager) [[unlikely]] {
			REX::Fail("Failed to get the game's message menu manager."sv);
		}

		REX::LogError("{}"sv,
			a_message);

		const auto buttons = [](bool a_addCancelButton) -> RE::BSTArray<RE::BSString> {
			if (a_addCancelButton) {
				return { CANCELABLE_BUTTON_ARRAY.begin(), CANCELABLE_BUTTON_ARRAY.end() };
			}

			return { DEFAULT_BUTTON_ARRAY.begin(), DEFAULT_BUTTON_ARRAY.end() };
		}(a_addCancelButton);

		auto messageCallback = REX::NotNull(RE::make_shared<ErrorMessageBoxCallback>(a_addCancelButton));
		messageManager->CreateCustomMessage(
			a_message.data(),
			messageCallback.get().get(),
			buttons,
			RE::WARNING_TYPES::kMasterFile);

		return messageCallback;
	}

	auto RequirementRunner::ModuleInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<ModuleInfo, std::string>
	{
		const auto nameIt = a_json.find(NAME_KEY);
		if (nameIt == a_json.end() || !nameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}" in array for key "{}")"sv,
				NAME_KEY, MODULES_KEY);
		}

		const auto fileVersionIt = a_json.find(FILE_VERSION_KEY);
		if (fileVersionIt != a_json.end() && !fileVersionIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}" in array for key "{}")"sv,
				FILE_VERSION_KEY, MODULES_KEY);
		}

		const auto productVersionIt = a_json.find(PRODUCT_VERSION_KEY);
		if (productVersionIt != a_json.end() && !productVersionIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}" in array for key "{}")"sv,
				PRODUCT_VERSION_KEY, MODULES_KEY);
		}

		auto moduleInfo = ModuleInfo();
		moduleInfo.name = nameIt->get<std::string>();

		if (fileVersionIt != a_json.end()) {
			const auto fileVersion = REX::Version::FromString(fileVersionIt->get<std::string_view>());
			if (!fileVersion.has_value()) {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to parse file version from string value for key "{}" in array for key "{}")"sv,
					FILE_VERSION_KEY, MODULES_KEY);
			}

			moduleInfo.fileVersion = fileVersion.value();
		}

		if (productVersionIt != a_json.end()) {
			const auto productVersion = REX::Version::FromString(productVersionIt->get<std::string_view>());
			if (!productVersion.has_value()) {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to parse product version from string value for key "{}" in array for key "{}")"sv,
					PRODUCT_VERSION_KEY, MODULES_KEY);
			}

			moduleInfo.productVersion = productVersion.value();
		}

		return moduleInfo;
	}

	auto RequirementRunner::ModuleInfo::VerifyModuleName(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		const auto& moduleName = name;
		const auto& expectedFileVersion = fileVersion;
		const auto& expectedProductVersion = productVersion;

		if (REL::Module::IsModuleLoaded(moduleName)) [[likely]] {
			return std::nullopt;
		}

		if (expectedFileVersion.has_value()) {
			return REX::Format(
				"The module \"{}\" with minimum file version {} is required by \"{}\" but it was not found.\n"
				"Please ensure that the module is properly installed and loaded by the game."sv,
				moduleName, expectedFileVersion.value(), a_rootName);
		}

		if (expectedProductVersion.has_value()) {
			return REX::Format(
				"The module \"{}\" with minimum product version {} is required by \"{}\" but it was not found.\n"
				"Please ensure that the module is properly installed and loaded by the game."sv,
				moduleName, expectedProductVersion.value(), a_rootName);
		}

		return REX::Format(
			"The module \"{}\" is required by \"{}\" but it was not found.\n"
			"Please ensure that the module is properly installed and loaded by the game."sv,
			moduleName, a_rootName);
	}

	auto RequirementRunner::ModuleInfo::VerifyModuleFileVersion(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		const auto& moduleName = name;
		const auto& expectedFileVersion = fileVersion;

		if (!expectedFileVersion.has_value()) {
			return std::nullopt;
		}

		const auto actualFileVersion = REX::GetFileVersion(moduleName);
		if (!actualFileVersion.has_value()) [[unlikely]] {
			return REX::Format(
				"The module \"{}\" with minimum file version {} is required by \"{}\" but the actual version could not be determined.\n"
				"Please ensure that the module is properly installed and loaded by the game."sv,
				moduleName, expectedFileVersion.value(), a_rootName);
		}

		if (actualFileVersion.value() < expectedFileVersion.value()) [[unlikely]] {
			return REX::Format(
				"The module \"{}\" with minimum file version {} is required by \"{}\" but the actual version is {}.\n"
				"Please ensure that you are using the correct version of the module."sv,
				moduleName, expectedFileVersion.value(), a_rootName, actualFileVersion.value());
		}

		return std::nullopt;
	}

	auto RequirementRunner::ModuleInfo::VerifyModuleProductVersion(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		const auto& moduleName = name;
		const auto& expectedProductVersion = productVersion;

		if (!expectedProductVersion.has_value()) {
			return std::nullopt;
		}

		const auto actualProductVersion = REX::GetProductVersion(moduleName);
		if (!actualProductVersion.has_value()) [[unlikely]] {
			return REX::Format(
				"The module \"{}\" with minimum product version {} is required by \"{}\" but the actual version could not be determined.\n"
				"Please ensure that the module is properly installed and loaded by the game."sv,
				moduleName, expectedProductVersion.value(), a_rootName);
		}

		if (actualProductVersion.value() < expectedProductVersion.value()) [[unlikely]] {
			return REX::Format(
				"The module \"{}\" with minimum product version {} is required by \"{}\" but the actual version is {}.\n"
				"Please ensure that you are using the correct version of the module."sv,
				moduleName, expectedProductVersion.value(), a_rootName, actualProductVersion.value());
		}

		return std::nullopt;
	}

	auto RequirementRunner::ModuleInfo::VerifyModule(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		auto moduleNameError = VerifyModuleName(a_rootName);
		if (moduleNameError.has_value()) {
			return moduleNameError;
		}

		auto fileVersionError = VerifyModuleFileVersion(a_rootName);
		if (fileVersionError.has_value()) {
			return fileVersionError;
		}

		auto productVersionError = VerifyModuleProductVersion(a_rootName);
		if (productVersionError.has_value()) {
			return productVersionError;
		}

		return std::nullopt;
	}

	auto RequirementRunner::XsePluginInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<XsePluginInfo, std::string>
	{
		const auto nameIt = a_json.find(NAME_KEY);
		if (nameIt == a_json.end() || !nameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}" in array for key "{}")"sv,
				NAME_KEY, XSE_PLUGINS_KEY);
		}

		const auto versionIt = a_json.find(VERSION_KEY);
		if (versionIt != a_json.end() && !(versionIt->is_string() || versionIt->is_number_unsigned())) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string or unsigned integer value for key "{}" in array for key "{}")"sv,
				VERSION_KEY, XSE_PLUGINS_KEY);
		}

		auto pluginInfo = XsePluginInfo();
		pluginInfo.name = nameIt->get<std::string>();

		if (versionIt != a_json.end()) {
			if (versionIt->is_string()) {
				const auto pluginVersion = REX::Version::FromString(versionIt->get<std::string_view>());
				if (!pluginVersion.has_value()) {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to parse version from string value for key "{}" in array for key "{}")"sv,
						VERSION_KEY, XSE_PLUGINS_KEY);
				}

				pluginInfo.version = pluginVersion.value();
			}
			else if (versionIt->is_number_unsigned()) {
				pluginInfo.version = versionIt->get<std::uint32_t>();
			}
			else {
				std::unreachable();
			}
		}

		return pluginInfo;
	}

	auto RequirementRunner::XsePluginInfo::VerifyXsePluginName(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		const auto* pluginInfo = F4SE::GetPluginInfo(name.data());
		if (pluginInfo) [[likely]] {
			return std::nullopt;
		}

		const auto& pluginName = name;
		const auto& pluginVersion = version;

		if (!pluginVersion.has_value()) {
			return REX::Format(
				"The script extender plugin \"{}\" is required by \"{}\" but it was not found.\n"
				"Please ensure that the plugin is properly installed and loaded by the game."sv,
				pluginName, a_rootName);
		}

		if (std::holds_alternative<REX::Version>(pluginVersion.value())) {
			const auto expectedVersionValue = std::get<REX::Version>(pluginVersion.value());

			return REX::Format(
				"The script extender plugin \"{}\" with minimum version {} is required by \"{}\" but it was not found.\n"
				"Please ensure that the plugin is properly installed and loaded by the game."sv,
				pluginName, expectedVersionValue, a_rootName);
		}

		if (std::holds_alternative<std::uint32_t>(pluginVersion.value())) {
			const auto expectedVersionValue = std::get<std::uint32_t>(pluginVersion.value());

			return REX::Format(
				"The script extender plugin \"{}\" with minimum version number {} is required by \"{}\" but it was not found.\n"
				"Please ensure that the plugin is properly installed and loaded by the game."sv,
				pluginName, expectedVersionValue, a_rootName);
		}

		std::unreachable();
	}

	auto RequirementRunner::XsePluginInfo::VerifyXsePluginVersion(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		const auto* pluginInfo = F4SE::GetPluginInfo(name.data());
		if (!pluginInfo) {
			return std::nullopt;
		}

		if (!version.has_value()) {
			return std::nullopt;
		}

		const auto& expectedVersion = version.value();

		if (std::holds_alternative<REX::Version>(expectedVersion)) {
			const auto expectedVersionValue = std::get<REX::Version>(expectedVersion);
			const auto actualVersionValue = pluginInfo->GetPluginVersion();

			if (actualVersionValue < expectedVersionValue) [[unlikely]] {
				return REX::Format(
					"The script extender plugin \"{}\" with minimum version {} is required by \"{}\" but the actual version is {}.\n"
					"Please ensure that you are using the correct version of the plugin."sv,
					name, expectedVersionValue, a_rootName, actualVersionValue);
			}
		}

		if (std::holds_alternative<std::uint32_t>(expectedVersion)) {
			const auto expectedVersionValue = std::get<std::uint32_t>(expectedVersion);
			const auto actualVersionValue = pluginInfo->GetPluginVersionNumber();

			if (actualVersionValue < expectedVersionValue) [[unlikely]] {
				return REX::Format(
					"The script extender plugin \"{}\" with minimum version number {} is required by \"{}\" but the actual version is {}.\n"
					"Please ensure that you are using the correct version of the plugin."sv,
					name, expectedVersionValue, a_rootName, actualVersionValue);
			}
		}

		return std::nullopt;
	}

	auto RequirementRunner::XsePluginInfo::VerifyXsePlugin(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		auto pluginNameError = VerifyXsePluginName(a_rootName);
		if (pluginNameError.has_value()) {
			return pluginNameError;
		}

		auto pluginVersionError = VerifyXsePluginVersion(a_rootName);
		if (pluginVersionError.has_value()) {
			return pluginVersionError;
		}

		return std::nullopt;
	}

	auto RequirementRunner::RequirementInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RequirementInfo, std::string>
	{
		const auto gameVersionIt = a_json.find(GAME_VERSION_KEY);
		if (gameVersionIt != a_json.end() && !gameVersionIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				GAME_VERSION_KEY);
		}

		const auto gamePluginsIt = a_json.find(GAME_PLUGINS_KEY);
		if (gamePluginsIt != a_json.end() && !gamePluginsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				GAME_PLUGINS_KEY);
		}

		const auto xseVersionIt = a_json.find(XSE_VERSION_KEY);
		if (xseVersionIt != a_json.end() && !xseVersionIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				XSE_VERSION_KEY);
		}

		const auto xsePluginsIt = a_json.find(XSE_PLUGINS_KEY);
		if (xsePluginsIt != a_json.end() && !xsePluginsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				XSE_PLUGINS_KEY);
		}

		const auto modulesIt = a_json.find(MODULES_KEY);
		if (modulesIt != a_json.end() && !modulesIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				MODULES_KEY);
		}

		if (gameVersionIt == a_json.end() &&
			gamePluginsIt == a_json.end() &&
			xseVersionIt == a_json.end() &&
			xsePluginsIt == a_json.end() &&
			modulesIt == a_json.end()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(at least one of the keys "{}", "{}", "{}", "{}", or "{}" must be present)"sv,
				GAME_VERSION_KEY, GAME_PLUGINS_KEY, XSE_VERSION_KEY, XSE_PLUGINS_KEY, MODULES_KEY);
		}

		auto requirementInfo = RequirementInfo();

		if (gameVersionIt != a_json.end()) {
			const auto gameVersion = REX::Version::FromString(gameVersionIt->get<std::string_view>());
			if (!gameVersion.has_value()) {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to parse game version from string value for key "{}")"sv,
					GAME_VERSION_KEY);
			}

			requirementInfo.gameVersion = gameVersion.value();
		}

		if (gamePluginsIt != a_json.end()) {
			auto gamePlugins = std::vector<std::string>();
			gamePlugins.reserve(gamePluginsIt->size());

			for (auto gamePluginIt = gamePluginsIt->begin(); gamePluginIt != gamePluginsIt->end(); gamePluginIt++) {
				if (!gamePluginIt->is_string()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find string element at index {} in array for key "{}")"sv,
						std::distance(gamePluginsIt->begin(), gamePluginIt), GAME_PLUGINS_KEY);
				}

				gamePlugins.push_back(gamePluginIt->get<std::string>());
			}

			requirementInfo.gamePlugins = std::move(gamePlugins);
		}

		if (xseVersionIt != a_json.end()) {
			const auto xseVersion = REX::Version::FromString(xseVersionIt->get<std::string_view>());
			if (!xseVersion.has_value()) {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to parse xse version from string value for key "{}")"sv,
					XSE_VERSION_KEY);
			}

			requirementInfo.xseVersion = xseVersion.value();
		}

		if (xsePluginsIt != a_json.end()) {
			auto xsePlugins = std::vector<REX::NotNull<std::shared_ptr<XsePluginInfo>>>();
			xsePlugins.reserve(xsePluginsIt->size());

			for (auto xsePluginIt = xsePluginsIt->begin(); xsePluginIt != xsePluginsIt->end(); xsePluginIt++) {
				if (!xsePluginIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(xsePluginsIt->begin(), xsePluginIt), XSE_PLUGINS_KEY);
				}

				auto xsePlugin = XsePluginInfo::FromJson(*xsePluginIt);
				if (!xsePlugin.has_value()) {
					return std::unexpected(std::move(xsePlugin).error());
				}

				xsePlugins.emplace_back(std::make_shared<XsePluginInfo>(std::move(xsePlugin).value()));
			}

			requirementInfo.xsePlugins = std::move(xsePlugins);
		}

		if (modulesIt != a_json.end()) {
			auto modules = std::vector<REX::NotNull<std::shared_ptr<ModuleInfo>>>();
			modules.reserve(modulesIt->size());

			for (auto moduleIt = modulesIt->begin(); moduleIt != modulesIt->end(); moduleIt++) {
				if (!moduleIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(modulesIt->begin(), moduleIt), MODULES_KEY);
				}

				auto moduleInfo = ModuleInfo::FromJson(*moduleIt);
				if (!moduleInfo.has_value()) {
					return std::unexpected(std::move(moduleInfo).error());
				}

				modules.emplace_back(std::make_shared<ModuleInfo>(std::move(moduleInfo).value()));
			}

			requirementInfo.modules = std::move(modules);
		}

		return requirementInfo;
	}

	auto RequirementRunner::RequirementInfo::VerifyGameVersion(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		if (!gameVersion.has_value()) {
			return std::nullopt;
		}

		const auto expectedVersion = gameVersion.value();
		const auto actualVersion = F4SE::GetRuntimeVersion();

		if (actualVersion < expectedVersion) [[unlikely]] {
			return REX::Format(
				"The game with minimum version {} is required by \"{}\" but the actual version is {}.\n"
				"Please ensure that you are using the correct version of the game."sv,
				expectedVersion, a_rootName, actualVersion);
		}

		return std::nullopt;
	}

	auto RequirementRunner::RequirementInfo::VerifyGamePlugins(std::string_view a_rootName) const
		-> std::optional<std::vector<std::string>>
	{
		if (!gamePlugins.has_value()) {
			return std::nullopt;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Fail("Failed to get the game's data handler."sv);
		}

		auto errorMessages = std::vector<std::string>();

		for (const auto& pluginName : gamePlugins.value()) {
			if (!dataHandler->IsFileLoaded(pluginName)) [[unlikely]] {
				errorMessages.push_back(REX::Format(
					"The game plugin \"{}\" is required by \"{}\" but it was not found.\n"
					"Please ensure that the plugin is properly installed and loaded by the game."sv,
					pluginName, a_rootName));
			}
		}

		return errorMessages;
	}

	auto RequirementRunner::RequirementInfo::VerifyXseVersion(std::string_view a_rootName) const
		-> std::optional<std::string>
	{
		if (!xseVersion.has_value()) {
			return std::nullopt;
		}

		const auto expectedVersion = xseVersion.value();
		const auto actualVersion = F4SE::GetF4SEVersion();

		if (actualVersion < expectedVersion) [[unlikely]] {
			return REX::Format(
				"The script extender with minimum version {} is required by \"{}\" but the actual version is {}.\n"
				"Please ensure that you are using the correct version of the script extender."sv,
				expectedVersion, a_rootName, actualVersion);
		}

		return std::nullopt;
	}

	auto RequirementRunner::RequirementInfo::VerifyXsePlugins(std::string_view a_rootName) const
		-> std::optional<std::vector<std::string>>
	{
		if (!xsePlugins.has_value()) {
			return std::nullopt;
		}

		auto errorMessages = std::vector<std::string>();

		for (const auto& xsePlugin : xsePlugins.value()) {
			auto errorMessage = xsePlugin->VerifyXsePlugin(a_rootName);
			if (errorMessage.has_value()) {
				errorMessages.push_back(std::move(errorMessage).value());
			}
		}

		return errorMessages;
	}

	auto RequirementRunner::RequirementInfo::VerifyModules(std::string_view a_rootName) const
		-> std::optional<std::vector<std::string>>
	{
		if (!modules.has_value()) {
			return std::nullopt;
		}

		auto errorMessages = std::vector<std::string>();

		for (const auto& module : modules.value()) {
			auto errorMessage = module->VerifyModule(a_rootName);
			if (errorMessage.has_value()) {
				errorMessages.push_back(std::move(errorMessage).value());
			}
		}

		return errorMessages;
	}

	auto RequirementRunner::RootInfo::VerifyAll() const
		-> std::vector<std::string>
	{
		auto errorMessages = std::vector<std::string>();

		auto gameVersionError = requirement.VerifyGameVersion(name);
		if (gameVersionError.has_value()) {
			errorMessages.push_back(std::move(gameVersionError).value());
		}

		auto gamePluginsError = requirement.VerifyGamePlugins(name);
		if (gamePluginsError.has_value()) {
			errorMessages.append_range(std::move(gamePluginsError).value());
		}

		auto xseVersionError = requirement.VerifyXseVersion(name);
		if (xseVersionError.has_value()) {
			errorMessages.push_back(std::move(xseVersionError).value());
		}

		auto xsePluginsError = requirement.VerifyXsePlugins(name);
		if (xsePluginsError.has_value()) {
			errorMessages.append_range(std::move(xsePluginsError).value());
		}

		auto modulesError = requirement.VerifyModules(name);
		if (modulesError.has_value()) {
			errorMessages.append_range(std::move(modulesError).value());
		}

		return errorMessages;
	}

	auto RequirementRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto nameIt = a_json.find(NAME_KEY);
		if (nameIt == a_json.end() || !nameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				NAME_KEY);
		}

		const auto requirementsIt = a_json.find(REQUIREMENTS_KEY);
		if (requirementsIt == a_json.end() || !requirementsIt->is_object()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find object for key "{}")"sv,
				REQUIREMENTS_KEY);
		}

		auto requirementInfo = RequirementInfo::FromJson(*requirementsIt);
		if (!requirementInfo.has_value()) {
			return std::unexpected(std::move(requirementInfo).error());
		}

		auto rootInfo = RootInfo();
		rootInfo.name = nameIt->get<std::string>();
		rootInfo.requirement = std::move(requirementInfo).value();

		return rootInfo;
	}

	RequirementRunner::RequirementRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _settingsManager(a_settingsManager)
	{
	}

	RequirementRunner::~RequirementRunner() noexcept
	{
		Dispose();
	}

	auto RequirementRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto filePaths = IFileRunner::LoadDirectoryFiles();
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			REX::Fail(R"(Failed to iterate directory "{}": {})"sv,
				_directoryPath.generic_string(), filePaths.error());
		}

		return a_threadPool.SubmitTaskRange(std::move(filePaths).value(), [this](const std::filesystem::path& filePath) {
			auto jsonRoot = IFileRunner::json_t();

			try {
				auto fileStream = std::fstream(filePath, std::ios::in);
				jsonRoot = IFileRunner::json_t::parse(fileStream,
					REX::Json::GetParserCallback<IFileRunner::json_t>(),
					REX::Json::ALLOW_EXCEPTIONS,
					REX::Json::IGNORE_COMMENTS);
			}
			catch (const IFileRunner::json_t::exception& error) {
				REX::LogError(R"(Failed to parse file "{}": {})"sv,
					filePath.generic_string(), error.what());
				return;
			}

			if (jsonRoot.empty()) {
				return;
			}

			auto rootInfo = RootInfo::FromJson(jsonRoot);
			if (!rootInfo.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					filePath.generic_string(), rootInfo.error());
				return;
			}

			REX::LogDebug(R"(Loaded file "{}")"sv,
				filePath.generic_string());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto RequirementRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto addCancelButton = _settingsManager->GetAddRequirementRunnerCancelButtonSetting()->GetValue();
		const auto runnerLock = std::scoped_lock(_mapMutex);

		auto errorMessageMap = std::multimap<std::filesystem::path, std::string>();
		for (const auto& [filePath, rootInfo] : _rootMap) {
			auto errorMessages = rootInfo->VerifyAll();

			REX::LogTrace(R"(Verified requirements for file "{}")"sv,
				filePath.generic_string());

			for (auto& errorMessage : errorMessages) {
				errorMessageMap.emplace(filePath, std::move(errorMessage));
			}
		}

		auto messageBoxCallbackMap = std::multimap<std::filesystem::path, REX::NotNull<RE::BSTSmartPointer<ErrorMessageBoxCallback>>>();
		for (const auto& [filePath, errorMessage] : errorMessageMap) {
			auto messageBoxCallback = ErrorMessageBoxCallback::CreateMessage(errorMessage, addCancelButton);
			messageBoxCallbackMap.emplace(filePath, std::move(messageBoxCallback));
		}

		for (const auto& [_, messageBoxCallback] : messageBoxCallbackMap) {
			if (!messageBoxCallback->TryWaitForMenuOpen()) {
				break;
			}

			messageBoxCallback->WaitForButtonClick();
		}

		return {};
	}

	void RequirementRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}
}
