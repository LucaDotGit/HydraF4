#pragma once

namespace Plugin::Internal
{
	class HooksManager final
	{
	public:
		HooksManager() = default;
		~HooksManager() noexcept = default;

		HooksManager(const HooksManager&) = delete;
		HooksManager(HooksManager&&) = delete;

		HooksManager& operator=(const HooksManager&) = delete;
		HooksManager& operator=(HooksManager&&) = delete;

		void OnXseLoad();
	};
}
