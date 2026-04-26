#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class WaitForNextFrameDelayFunctor final
		: public ISharedDelayFunctor<WaitForNextFrameDelayFunctor>
	{
	public:
		explicit WaitForNextFrameDelayFunctor(RE::BSScript::StackID a_stackId);
		explicit WaitForNextFrameDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~WaitForNextFrameDelayFunctor() noexcept override;

		WaitForNextFrameDelayFunctor(const WaitForNextFrameDelayFunctor&) = delete;
		WaitForNextFrameDelayFunctor(WaitForNextFrameDelayFunctor&&) = delete;

		WaitForNextFrameDelayFunctor& operator=(const WaitForNextFrameDelayFunctor&) = delete;
		WaitForNextFrameDelayFunctor& operator=(WaitForNextFrameDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] RE::BSScript::StackID GetStackId() const noexcept { return _stackId; }

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
		bool ShouldReschedule(std::int32_t& a_outDelayMS) override;
		bool ShouldResumeStack(RE::BSScript::StackID& a_outStackId) override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

	private:
		RE::BSScript::StackID _stackId{ F4SE::IDelayFunctor::EMPTY_STACK_ID };
	};

	using WaitForNextFrameDelayFunctorFactory = F4SE::SharedObjectFactory<WaitForNextFrameDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_WaitForNextFrameDelayFunctorFactory">;
}
