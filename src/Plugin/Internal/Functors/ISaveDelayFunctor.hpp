#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class ISaveDelayFunctor
		: public ISharedDelayFunctor<ISaveDelayFunctor>
	{
	public:
		explicit ISaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit ISaveDelayFunctor(RE::BSFixedString a_saveName, bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit ISaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~ISaveDelayFunctor() noexcept override;

		ISaveDelayFunctor(const ISaveDelayFunctor&) = delete;
		ISaveDelayFunctor(ISaveDelayFunctor&&) = delete;

		ISaveDelayFunctor& operator=(const ISaveDelayFunctor&) = delete;
		ISaveDelayFunctor& operator=(ISaveDelayFunctor&&) = delete;

		[[nodiscard]] bool GetIsSilent() const noexcept { return _isSilent; }
		[[nodiscard]] RE::BSScript::StackID GetStackId() const noexcept { return _stackId; }

		bool ShouldReschedule(std::int32_t& a_outDelayMS) override;
		bool ShouldResumeStack(RE::BSScript::StackID& a_outStackId) override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

	protected:
		RE::BSFixedString _saveName;
		bool _isSilent{ false };
		RE::BSScript::StackID _stackId{ F4SE::IDelayFunctor::EMPTY_STACK_ID };
	};
}
