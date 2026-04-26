#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class PostSaveGameEvent final
	{
	public:
		RE::BSFixedString saveName;
	};

	extern template class IEventSource<PostSaveGameEvent>;
	using PostSaveGameSource = IEventSource<PostSaveGameEvent>;
}
