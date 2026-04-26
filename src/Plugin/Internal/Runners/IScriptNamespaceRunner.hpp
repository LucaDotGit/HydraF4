#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Save
{
	class ISaveScriptVariantCollection;
}

namespace Plugin::Internal::Runners
{
	class IScriptNamespaceRunner
		: public IFileRunner
	{
	public:
		class NamespaceInfo final
		{
		public:
			NamespaceInfo() = default;
			~NamespaceInfo() noexcept = default;

			NamespaceInfo(const NamespaceInfo&) = delete;
			NamespaceInfo(NamespaceInfo&&) noexcept = default;

			NamespaceInfo& operator=(const NamespaceInfo&) = delete;
			NamespaceInfo& operator=(NamespaceInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json)
				-> std::expected<NamespaceInfo, std::string>;

			std::string name;
		};

		class RootInfo final
		{
		public:
			RootInfo() = default;
			~RootInfo() noexcept = default;

			RootInfo(const RootInfo&) = delete;
			RootInfo(RootInfo&&) noexcept = default;

			RootInfo& operator=(const RootInfo&) = delete;
			RootInfo& operator=(RootInfo&&) noexcept = default;

			[[nodiscard]] static auto FromJson(const IFileRunner::json_t& a_json, std::string_view a_rootName)
				-> std::expected<RootInfo, std::string>;

			std::vector<NamespaceInfo> names;
		};

		IScriptNamespaceRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension,
			const REX::NotNull<std::shared_ptr<Save::ISaveScriptVariantCollection>>& a_saveScriptVariantCollection);

		~IScriptNamespaceRunner() noexcept override;

		IScriptNamespaceRunner(const IScriptNamespaceRunner&) = delete;
		IScriptNamespaceRunner(IScriptNamespaceRunner&&) = delete;

		IScriptNamespaceRunner& operator=(const IScriptNamespaceRunner&) = delete;
		IScriptNamespaceRunner& operator=(IScriptNamespaceRunner&&) = delete;

		[[nodiscard]] virtual std::string_view GetRootName() const noexcept = 0;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	protected:
		REX::NotNull<std::shared_ptr<Save::ISaveScriptVariantCollection>> _saveScriptVariantCollection;
		mutable std::mutex _mapMutex;
		std::multimap<std::filesystem::path, RE::BSFixedString> _validNamespaceMap;
	};
}
