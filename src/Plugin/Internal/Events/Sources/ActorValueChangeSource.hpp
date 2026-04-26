#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class ActorValueChangeEvent final
	{
	public:
		RE::NiPointer<RE::TESObjectREFR> sourceRef;
		RE::ActorValueInfo* sourceValue;
		REX::Float32 oldValue;
		REX::Float32 newValue;
	};

	extern template class IEventSource<ActorValueChangeEvent>;
	using ActorValueChangeSource = IEventSource<ActorValueChangeEvent>;
}
