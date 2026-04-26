#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class NewGameEvent final
	{
	public:
		RE::TESQuest* charGenQuest;
	};

	extern template class IEventSource<NewGameEvent>;
	using NewGameSource = IEventSource<NewGameEvent>;
}
