#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	template <class EventT>
	auto IEventSource<EventT>::GetEventSource() -> const REX::NotNull<std::unique_ptr<IEventSource>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<IEventSource>());
		return INSTANCE;
	}
}
