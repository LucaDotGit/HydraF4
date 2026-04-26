#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Runners
{
	class RequirementRunner final
		: public IFileRunner
	{
	public:
		class ErrorMessageBoxCallback final
			: public RE::IMessageBoxCallback,
			  public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			struct ButtonIndexes
			{
				enum ButtonIndex : std::uint8_t
				{
					Abort = 0,
					Cancel = 1
				};
			};
			using ButtonIndex = ButtonIndexes::ButtonIndex;

			inline static constexpr auto ABORT_TRANSLATION_KEY = "$ABORT"sv;
			inline static constexpr auto CANCEL_TRANSLATION_KEY = "$CANCEL"sv;

			inline static constexpr auto DEFAULT_BUTTON_ARRAY = std::array{
				ABORT_TRANSLATION_KEY
			};

			inline static constexpr auto CANCELABLE_BUTTON_ARRAY = std::array{
				ABORT_TRANSLATION_KEY,
				CANCEL_TRANSLATION_KEY
			};

			inline static constexpr auto MENU_OPEN_TIMEOUT_DELAY = 200ms;

			ErrorMessageBoxCallback(bool a_hasCancelButton);
			~ErrorMessageBoxCallback() noexcept override;

			ErrorMessageBoxCallback(const ErrorMessageBoxCallback&) = delete;
			ErrorMessageBoxCallback(ErrorMessageBoxCallback&&) = delete;

			ErrorMessageBoxCallback& operator=(const ErrorMessageBoxCallback&) = delete;
			ErrorMessageBoxCallback& operator=(ErrorMessageBoxCallback&&) = delete;

			void operator()(std::uint8_t a_buttonIndex) override;

			RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event,
				RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

			[[nodiscard]] bool HasCancelButton() const noexcept { return _hasCancelButton; }

			[[nodiscard]] bool TryWaitForMenuOpen() noexcept;
			void WaitForButtonClick() noexcept;

			void RegisterEvents();
			void UnregisterEvents();

			[[nodiscard]] static auto CreateMessage(REX::zstring_view a_message, bool a_addCancelButton)
				-> REX::NotNull<RE::BSTSmartPointer<ErrorMessageBoxCallback>>;

		private:
			std::binary_semaphore _menuOpenSignal{ 0 };
			std::binary_semaphore _buttonClickSignal{ 0 };
			bool _hasCancelButton{ false };
		};

		class ModuleInfo final
			: public std::enable_shared_from_this<ModuleInfo>
		{
		public:
			ModuleInfo() = default;
			~ModuleInfo() noexcept = default;

			ModuleInfo(const ModuleInfo&) = default;
			ModuleInfo(ModuleInfo&&) noexcept = default;

			ModuleInfo& operator=(const ModuleInfo&) = default;
			ModuleInfo& operator=(ModuleInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<ModuleInfo, std::string>;

			auto VerifyModuleName(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyModuleFileVersion(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyModuleProductVersion(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyModule(std::string_view a_rootName) const
				-> std::optional<std::string>;

			std::string name;
			std::optional<REX::Version> fileVersion;
			std::optional<REX::Version> productVersion;
		};

		class XsePluginInfo final
			: public std::enable_shared_from_this<XsePluginInfo>
		{
		public:
			XsePluginInfo() = default;
			~XsePluginInfo() noexcept = default;

			XsePluginInfo(const XsePluginInfo&) = default;
			XsePluginInfo(XsePluginInfo&&) noexcept = default;

			XsePluginInfo& operator=(const XsePluginInfo&) = default;
			XsePluginInfo& operator=(XsePluginInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<XsePluginInfo, std::string>;

			auto VerifyXsePluginName(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyXsePluginVersion(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyXsePlugin(std::string_view a_rootName) const
				-> std::optional<std::string>;

			std::string name;
			std::optional<std::variant<REX::Version, std::uint32_t>> version;
		};

		class RequirementInfo final
			: public std::enable_shared_from_this<RequirementInfo>
		{
		public:
			RequirementInfo() = default;
			~RequirementInfo() noexcept = default;

			RequirementInfo(const RequirementInfo&) = default;
			RequirementInfo(RequirementInfo&&) noexcept = default;

			RequirementInfo& operator=(const RequirementInfo&) = default;
			RequirementInfo& operator=(RequirementInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<RequirementInfo, std::string>;

			auto VerifyGameVersion(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyGamePlugins(std::string_view a_rootName) const
				-> std::optional<std::vector<std::string>>;
			auto VerifyXseVersion(std::string_view a_rootName) const
				-> std::optional<std::string>;
			auto VerifyXsePlugins(std::string_view a_rootName) const
				-> std::optional<std::vector<std::string>>;
			auto VerifyModules(std::string_view a_rootName) const
				-> std::optional<std::vector<std::string>>;

			std::optional<REX::Version> gameVersion;
			std::optional<std::vector<std::string>> gamePlugins;
			std::optional<REX::Version> xseVersion;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<XsePluginInfo>>>> xsePlugins;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<ModuleInfo>>>> modules;
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

			[[nodiscard]] auto VerifyAll() const
				-> std::vector<std::string>;

			std::string name;
			RequirementInfo requirement;
		};

		RequirementRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);

		~RequirementRunner() noexcept override;

		RequirementRunner(const RequirementRunner&) = delete;
		RequirementRunner(RequirementRunner&&) = delete;

		RequirementRunner& operator=(const RequirementRunner&) = delete;
		RequirementRunner& operator=(RequirementRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
