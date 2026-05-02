#include "Plugin/Internal/Save/SaveEventSystem.hpp"

#include "Plugin/Internal/Events/EventFunctorMap.hpp"
#include "Plugin/Internal/Events/EventSystem.hpp"
#include "Plugin/Internal/Events/IEventHandler.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveEventSystem::SaveEventSystem(std::uint32_t a_type, std::uint32_t a_version,
		const REX::NotNull<std::shared_ptr<Events::EventSystem>>& a_eventSystem)
		: ISaveObject(a_type, a_version),
		  _eventSystem(a_eventSystem)
	{
	}

	SaveEventSystem::~SaveEventSystem() noexcept = default;

	bool SaveEventSystem::Load(const SaveStream& a_stream)
	{
		while (!a_stream.GetMemoryStream().at_end()) {
			auto eventName = RE::BSFixedStringCS();
			if (!a_stream.ReadString(eventName)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			const auto eventFunctor = Events::GetEventFunctorMap()->GetValue(eventName);
			if (!eventFunctor) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			auto eventHandler = std::shared_ptr<const Events::IEventHandler>();
			if (!std::invoke(*(eventFunctor->GetLoadCallback()), a_stream, eventHandler)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!eventHandler) {
				continue;
			}

			if (!_eventSystem->Register(eventHandler->GetFunctionRef(), eventHandler)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}
		}

		return true;
	}

	bool SaveEventSystem::Save(SaveStream& a_stream) const
	{
		REX::Assert(a_stream.GetMemoryStream().empty());

		return _eventSystem->ForEach([&a_stream](const REX::NotNull<std::shared_ptr<const Events::IEventHandler>>& a_eventHandler) {
			if (!a_eventHandler->GetIsPersistent()) {
				return true;
			}

			const auto eventName = RE::BSFixedStringCS(a_eventHandler->GetName());
			if (!a_stream.WriteString(eventName)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			const auto eventFunctor = Events::GetEventFunctorMap()->GetValue(eventName);
			if (!eventFunctor) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!std::invoke(*(eventFunctor->GetSaveCallback()), a_stream, a_eventHandler)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			return true;
		});
	}

	void SaveEventSystem::Revert() noexcept
	{
		_eventSystem->Revert();
	}
}
