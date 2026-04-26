#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class HudColorUpdateEvent final
	{
	public:
		std::uint8_t red;
		std::uint8_t green;
		std::uint8_t blue;
	};

	extern template class IEventSource<HudColorUpdateEvent>;
	using HudColorUpdateSource = IEventSource<HudColorUpdateEvent>;
}
