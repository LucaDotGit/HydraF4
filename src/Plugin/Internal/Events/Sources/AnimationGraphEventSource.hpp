#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class AnimationGraphEvent final
	{
	public:
		RE::NiPointer<RE::TESObjectREFR> sourceRef;
		RE::BSFixedString eventName;
		RE::BSFixedString payload;
	};

	extern template class IEventSource<AnimationGraphEvent>;
	using AnimationGraphEventSource = IEventSource<AnimationGraphEvent>;
}
