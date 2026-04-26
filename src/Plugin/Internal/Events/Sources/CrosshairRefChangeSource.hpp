#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class CrosshairRefChangeEvent final
	{
	public:
		RE::NiPointer<RE::TESObjectREFR> targetRef;
		bool isTargeted;
	};

	extern template class IEventSource<CrosshairRefChangeEvent>;
	using CrosshairRefChangeSource = IEventSource<CrosshairRefChangeEvent>;
}
