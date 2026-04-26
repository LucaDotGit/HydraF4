#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class ILoadDelayFunctor
		: public ISharedDelayFunctor<ILoadDelayFunctor>
	{
	public:
		explicit ILoadDelayFunctor(RE::BSFixedString a_saveName, bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit ILoadDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~ILoadDelayFunctor() noexcept override;

		ILoadDelayFunctor(const ILoadDelayFunctor&) = delete;
		ILoadDelayFunctor(ILoadDelayFunctor&&) = delete;

		ILoadDelayFunctor& operator=(const ILoadDelayFunctor&) = delete;
		ILoadDelayFunctor& operator=(ILoadDelayFunctor&&) = delete;

		[[nodiscard]] const RE::BSFixedString& GetSaveName() const noexcept { return _saveName; }
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
