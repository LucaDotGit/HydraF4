#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Save
{
	class SaveScriptObjectMap;
}

namespace Plugin::Internal::Runners
{
	class ScriptObjectRunner final
		: public IFileRunner
	{
	public:
		class ObjectInfo final
			: public std::enable_shared_from_this<ObjectInfo>
		{
		public:
			ObjectInfo() = default;
			~ObjectInfo() noexcept = default;

			ObjectInfo(const ObjectInfo&) = default;
			ObjectInfo(ObjectInfo&&) noexcept = default;

			ObjectInfo& operator=(const ObjectInfo&) = default;
			ObjectInfo& operator=(ObjectInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<ObjectInfo, std::string>;

			std::string boundObject;
			std::optional<std::unordered_map<std::string, IFileRunner::json_t>> properties;
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

			std::vector<REX::NotNull<std::shared_ptr<ObjectInfo>>> objects;
		};

		ScriptObjectRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<Save::SaveScriptObjectMap>>& a_saveScriptObjectMap);

		~ScriptObjectRunner() noexcept override;

		ScriptObjectRunner(const ScriptObjectRunner&) = delete;
		ScriptObjectRunner(ScriptObjectRunner&&) = delete;

		ScriptObjectRunner& operator=(const ScriptObjectRunner&) = delete;
		ScriptObjectRunner& operator=(ScriptObjectRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		auto RunScript(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const REX::NotNull<std::shared_ptr<ObjectInfo>>& a_script)
			-> std::optional<std::string>;
		void RunScripts(
			const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<ObjectInfo>>> a_scripts);

		REX::NotNull<std::shared_ptr<Save::SaveScriptObjectMap>> _saveScriptObjectMap;
		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
