#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class LifeStateChangeEvent final
	{
	public:
		RE::NiPointer<RE::Actor> actor;
		REX::Enum<RE::ACTOR_LIFE_STATE> oldState;
		REX::Enum<RE::ACTOR_LIFE_STATE> newState;
	};

	extern template class IEventSource<LifeStateChangeEvent>;
	using LifeStateChangeSource = IEventSource<LifeStateChangeEvent>;
}
