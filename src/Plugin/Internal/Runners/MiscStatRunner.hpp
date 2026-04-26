#pragma once

#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	class MiscStatRunner final
		: public IFileRunner
	{
	public:
		using MiscStatType = RE::MiscStatManager::MiscStatType;

		inline static constexpr auto MISC_STAT_TYPE_MAP = REX::StaticMap<std::string_view, MiscStatType, 9>{
			{ "General"sv, MiscStatType::kGeneral },
			{ "Quest"sv, MiscStatType::kQuest },
			{ "Combat"sv, MiscStatType::kCombat },
			{ "Crafting"sv, MiscStatType::kCrafting },
			{ "Crime"sv, MiscStatType::kCrime },
			{ "Sanctuary"sv, MiscStatType::kSanctuary },
			{ "Log"sv, MiscStatType::kLog },
			{ "MiniGame"sv, MiscStatType::kMiniGame },
			{ "Hidden"sv, MiscStatType::kHidden }
		};

		class MiscStatInfo final
			: public std::enable_shared_from_this<MiscStatInfo>
		{
		public:
			MiscStatInfo() = default;
			~MiscStatInfo() noexcept = default;

			MiscStatInfo(const MiscStatInfo&) = default;
			MiscStatInfo(MiscStatInfo&&) noexcept = default;

			MiscStatInfo& operator=(const MiscStatInfo&) = default;
			MiscStatInfo& operator=(MiscStatInfo&&) noexcept = default;

			[[nodiscard]] static std::expected<MiscStatInfo, std::string> FromJson(const IFileRunner::json_t& a_json);

			std::string statId;
			std::string statName;
			std::string statType;
			std::optional<bool> showIfZero;
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

			std::vector<REX::NotNull<std::shared_ptr<MiscStatInfo>>> miscStats;
		};

		MiscStatRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		~MiscStatRunner() noexcept override;

		MiscStatRunner(const MiscStatRunner&) = delete;
		MiscStatRunner(MiscStatRunner&&) = delete;

		MiscStatRunner& operator=(const MiscStatRunner&) = delete;
		MiscStatRunner& operator=(MiscStatRunner&&) = delete;

		[[nodiscard]] auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		[[nodiscard]] auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> override;
		void Dispose() noexcept override;

	private:
		static auto RunMiscStat(
			RE::BSTArray<RE::MiscStatManager::MiscStat>& a_miscStatHolder,
			const REX::NotNull<std::shared_ptr<MiscStatInfo>>& a_miscStat)
			-> std::optional<std::string>;
		static void RunMiscStats(
			RE::BSTArray<RE::MiscStatManager::MiscStat>& a_miscStatHolder,
			const std::filesystem::path& a_filePath,
			std::span<const REX::NotNull<std::shared_ptr<MiscStatInfo>>> a_miscStats);

		mutable std::mutex _mapMutex;
		std::map<std::filesystem::path, REX::NotNull<std::shared_ptr<RootInfo>>> _rootMap;
	};
}
