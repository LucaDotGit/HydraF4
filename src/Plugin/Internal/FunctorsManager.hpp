#pragma once

namespace Plugin::Internal
{
	class FunctorsManager final
	{
	public:
		FunctorsManager() = default;
		~FunctorsManager() noexcept = default;

		FunctorsManager(const FunctorsManager&) = delete;
		FunctorsManager(FunctorsManager&&) = delete;

		FunctorsManager& operator=(const FunctorsManager&) = delete;
		FunctorsManager& operator=(FunctorsManager&&) = delete;

		void OnPostLoad();
		void OnGameDataReady();
		void OnSaveRevert();
	};
}
