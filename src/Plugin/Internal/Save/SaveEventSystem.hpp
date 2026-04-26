#pragma once

#include "Plugin/Internal/Save/ISaveObject.hpp"

namespace Plugin::Internal::Events
{
	class EventSystem;
}

namespace Plugin::Internal::Save
{
	class SaveEventSystem final
		: public ISaveObject
	{
	public:
		SaveEventSystem(std::uint32_t a_type, std::uint32_t a_version,
			const REX::NotNull<std::shared_ptr<Events::EventSystem>>& a_eventSystem);

		~SaveEventSystem() noexcept override;

		SaveEventSystem(const SaveEventSystem&) = delete;
		SaveEventSystem(SaveEventSystem&&) = delete;

		SaveEventSystem& operator=(const SaveEventSystem&) = delete;
		SaveEventSystem& operator=(SaveEventSystem&&) = delete;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

	private:
		REX::NotNull<std::shared_ptr<Events::EventSystem>> _eventSystem;
	};
}
