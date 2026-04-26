#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	class ScriptReflectionRunner final
		: public IFileRunner
	{
	public:
		class ReplacedFunctionInfo final
			: public std::enable_shared_from_this<ReplacedFunctionInfo>
		{
		public:
			ReplacedFunctionInfo() = default;
			~ReplacedFunctionInfo() noexcept = default;

			ReplacedFunctionInfo(const ReplacedFunctionInfo&) = default;
			ReplacedFunctionInfo(ReplacedFunctionInfo&&) noexcept = default;

			ReplacedFunctionInfo& operator=(const ReplacedFunctionInfo&) = default;
			ReplacedFunctionInfo& operator=(ReplacedFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<ReplacedFunctionInfo, std::string>;

			std::string oldScriptName;
			std::string oldFunctionName;
			std::optional<std::string> oldStateName;
			std::string newScriptName;
			std::string newFunctionName;
			std::optional<std::string> newStateName;
		};

		class IReplacedPropertyFunctionInfo
			: public std::enable_shared_from_this<IReplacedPropertyFunctionInfo>
		{
		public:
			IReplacedPropertyFunctionInfo() = default;
			virtual ~IReplacedPropertyFunctionInfo() noexcept = default;

			IReplacedPropertyFunctionInfo(const IReplacedPropertyFunctionInfo&) = default;
			IReplacedPropertyFunctionInfo(IReplacedPropertyFunctionInfo&&) noexcept = default;

			IReplacedPropertyFunctionInfo& operator=(const IReplacedPropertyFunctionInfo&) = default;
			IReplacedPropertyFunctionInfo& operator=(IReplacedPropertyFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJsonImpl(const IFileRunner::json_t& a_json, IReplacedPropertyFunctionInfo& a_functionInfo)
				-> std::optional<std::string>;

			std::string oldScriptName;
			std::string oldPropertyName;
			std::string newScriptName;
			std::string newPropertyName;
		};

		class ReplacedGetterFunctionInfo final
			: public IReplacedPropertyFunctionInfo
		{
		public:
			ReplacedGetterFunctionInfo() = default;
			~ReplacedGetterFunctionInfo() noexcept override = default;

			ReplacedGetterFunctionInfo(const ReplacedGetterFunctionInfo&) = default;
			ReplacedGetterFunctionInfo(ReplacedGetterFunctionInfo&&) noexcept = default;

			ReplacedGetterFunctionInfo& operator=(const ReplacedGetterFunctionInfo&) = default;
			ReplacedGetterFunctionInfo& operator=(ReplacedGetterFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<ReplacedGetterFunctionInfo, std::string>;
		};

		class ReplacedSetterFunctionInfo final
			: public IReplacedPropertyFunctionInfo
		{
		public:
			ReplacedSetterFunctionInfo() = default;
			~ReplacedSetterFunctionInfo() noexcept override = default;

			ReplacedSetterFunctionInfo(const ReplacedSetterFunctionInfo&) = default;
			ReplacedSetterFunctionInfo(ReplacedSetterFunctionInfo&&) noexcept = default;

			ReplacedSetterFunctionInfo& operator=(const ReplacedSetterFunctionInfo&) = default;
			ReplacedSetterFunctionInfo& operator=(ReplacedSetterFunctionInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<ReplacedSetterFunctionInfo, std::string>;
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

			std::optional<std::vector<REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>>> replacedFunctions;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>>> replacedGetterFunctions;
			std::optional<std::vector<REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>>> replacedSetterFunctions;
		};

		ScriptReflectionRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		~ScriptReflectionRunner() noexcept override;

		ScriptReflectionRunner(const ScriptReflectionRunner&) = delete;
		ScriptReflectionRunner(ScriptReflectionRunner&&) = delete;

		ScriptReflectionRunner& operator=(const ScriptReflectionRunner&) = delete;
		ScriptReflectionRunner& operator=(ScriptReflectionRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		static auto ReplaceFunction(
			RE::BSTSmartPointer<RE::BSScript::IFunction>& a_oldFunction,
			const RE::BSTSmartPointer<RE::BSScript::IFunction>& a_newFunction)
			-> std::optional<std::string>;

		static auto CopyByteCodeFunctionTo(
			const RE::BSTSmartPointer<RE::BSScript::IFunction>& a_sourceFunction,
			RE::BSTSmartPointer<RE::BSScript::IFunction>& a_targetFunction)
			-> std::optional<std::string>;

		static auto RunReplacedFunction(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>& a_replacedFunction)
			-> std::optional<std::string>;
		static void RunReplacedFunctions(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>> a_replacedFunctions);

		static auto RunReplacedGetterFunction(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>& a_replacedGetterFunction)
			-> std::optional<std::string>;
		static void RunReplacedGetterFunctions(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>> a_replacedGetterFunctions);

		static auto RunReplacedSetterFunction(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>& a_replacedSetterFunction)
			-> std::optional<std::string>;
		static void RunReplacedSetterFunctions(
			RE::BSScript::Internal::VirtualMachine& a_internalVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>> a_replacedSetterFunctions);

		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
