#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class StartGameEvent final
	{
	public:
		bool isSucceeded;
	};

	extern template class IEventSource<StartGameEvent>;
	using StartGameSource = IEventSource<StartGameEvent>;
}
