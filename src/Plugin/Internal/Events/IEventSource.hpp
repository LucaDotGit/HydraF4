#pragma once

namespace Plugin::Internal::Events
{
	template <class EventT>
	class IEventSource final
		: public RE::BSTEventSource<EventT>
	{
	public:
		using event_type = EventT;

		IEventSource() = default;
		~IEventSource() noexcept = default;

		IEventSource(const IEventSource&) = delete;
		IEventSource(IEventSource&&) = delete;

		IEventSource& operator=(const IEventSource&) = delete;
		IEventSource& operator=(IEventSource&&) = delete;

		[[nodiscard]] static auto GetEventSource() -> const REX::NotNull<std::unique_ptr<IEventSource>>&;
	};
}

#include "Plugin/Internal/Events/IEventSource.inl"
