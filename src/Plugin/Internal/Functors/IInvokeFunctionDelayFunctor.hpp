#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"

namespace Plugin::Internal::Save
{
	class SaveStream;
}

namespace Plugin::Internal::Functors
{
	class IInvokeFunctionDelayFunctor
		: public ISharedDelayFunctor<IInvokeFunctionDelayFunctor>
	{
	public:
		IInvokeFunctionDelayFunctor(RE::BSScript::StackID a_stackId);
		explicit IInvokeFunctionDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~IInvokeFunctionDelayFunctor() noexcept override;

		IInvokeFunctionDelayFunctor(const IInvokeFunctionDelayFunctor&) = delete;
		IInvokeFunctionDelayFunctor(IInvokeFunctionDelayFunctor&&) = delete;

		IInvokeFunctionDelayFunctor& operator=(const IInvokeFunctionDelayFunctor&) = delete;
		IInvokeFunctionDelayFunctor& operator=(IInvokeFunctionDelayFunctor&&) = delete;

		[[nodiscard]] RE::BSScript::StackID GetStackId() const noexcept { return _stackId; }
		[[nodiscard]] RE::BSScript::StackID GetCallbackStackId() const noexcept { return _callbackStackId; }
		[[nodiscard]] bool GetHasInvoked() const noexcept { return _hasInvoked; }
		[[nodiscard]] bool GetHasReturned() const noexcept;

		bool Run(RE::BSScript::Variable& a_outResult) override;
		bool ShouldReschedule(std::int32_t& a_outDelayMS) override;
		bool ShouldResumeStack(RE::BSScript::StackID& a_outStackId) override;

		virtual bool InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor) = 0;
		virtual void ReturnFunction(const RE::BSScript::Variable& a_returnValue);

	private:
		bool DoRunFunction();

	protected:
		bool RestoreStackCallback(RE::BSScript::StackID a_stackId);

		RE::BSScript::StackID _stackId{ F4SE::IDelayFunctor::EMPTY_STACK_ID };
		RE::BSScript::StackID _callbackStackId{ F4SE::IDelayFunctor::EMPTY_STACK_ID };
		bool _hasInvoked{ false };
		std::atomic<bool> _hasReturned{ false };
		bool _tempShouldResume{ false };
	};
}
