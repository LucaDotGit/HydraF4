#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class PostLoadGameEvent final
	{
	public:
		bool isSucceeded;
	};

	extern template class IEventSource<PostLoadGameEvent>;
	using PostLoadGameSource = IEventSource<PostLoadGameEvent>;
}
