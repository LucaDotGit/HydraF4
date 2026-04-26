#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class DeleteGameEvent final
	{
	public:
		RE::BSFixedString saveName;
	};

	extern template class IEventSource<DeleteGameEvent>;
	using DeleteGameSource = IEventSource<DeleteGameEvent>;
}
