#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class UserEvent final
	{
	public:
		RE::BSFixedString eventName;
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> args;
		bool isApplied;
	};

	extern template class IEventSource<UserEvent>;
	using UserEventSource = IEventSource<UserEvent>;
}
