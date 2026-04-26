#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptFunctionMap;
}

namespace Plugin::Internal::Runners
{
	class ScriptFunctionRunner final
		: public IFileRunner
	{
	public:
		class LocalFunctionInfo final
			: public std::enable_shared_from_this<LocalFunctionInfo>
		{
		public:
			LocalFunctionInfo() = default;
			~LocalFunctionInfo() noexcept = default;

			LocalFunctionInfo(const LocalFunctionInfo&) = default;
			LocalFunctionInfo(LocalFunctionInfo&&) noexcept = default;

			LocalFunctionInfo& operator=(const LocalFunctionInfo&) = default;
			LocalFunctionInfo& operator=(LocalFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<LocalFunctionInfo, std::string>;

			std::string eventName;
			std::string boundObject;
			std::string functionName;
			std::optional<std::unordered_map<std::string, IFileRunner::json_t>> eventArgs;
		};

		class GlobalFunctionInfo final
			: public std::enable_shared_from_this<GlobalFunctionInfo>
		{
		public:
			GlobalFunctionInfo() = default;
			~GlobalFunctionInfo() noexcept = default;

			GlobalFunctionInfo(const GlobalFunctionInfo&) = default;
			GlobalFunctionInfo(GlobalFunctionInfo&&) noexcept = default;

			GlobalFunctionInfo& operator=(const GlobalFunctionInfo&) = default;
			GlobalFunctionInfo& operator=(GlobalFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<GlobalFunctionInfo, std::string>;

			std::string eventName;
			std::string scriptName;
			std::string functionName;
			std::optional<std::unordered_map<std::string, IFileRunner::json_t>> eventArgs;
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

			std::optional<std::vector<REX::NotNull<std::shared_ptr<LocalFunctionInfo>>>> localFunctions;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>>> globalFunctions;
		};

		ScriptFunctionRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<Save::SaveScriptFunctionMap>>& a_saveScriptFunctionMap);

		~ScriptFunctionRunner() noexcept override;

		ScriptFunctionRunner(const ScriptFunctionRunner&) = delete;
		ScriptFunctionRunner(ScriptFunctionRunner&&) = delete;

		ScriptFunctionRunner& operator=(const ScriptFunctionRunner&) = delete;
		ScriptFunctionRunner& operator=(ScriptFunctionRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		auto RunLocalFunction(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const REX::NotNull<std::shared_ptr<LocalFunctionInfo>>& a_function)
			-> std::optional<std::string>;
		void RunLocalFunctions(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<LocalFunctionInfo>>> a_functions);

		auto RunGlobalFunction(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>& a_function)
			-> std::optional<std::string>;
		void RunGlobalFunctions(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>> a_functions);

		REX::NotNull<std::shared_ptr<Save::SaveScriptFunctionMap>> _saveScriptFunctionMap;
		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
